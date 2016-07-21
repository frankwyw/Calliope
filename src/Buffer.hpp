#ifndef __BUFFER__H
#define __BUFFER__H

#include <vector>
#include <mutex>
#include <memory>

#include <unistd.h>

#include "Socket.h"


namespace honoka
{
    template<typename T>
    class Ring_buffer
    {
    public:

        Ring_buffer():max_size_(4000), size_(0), mutex_(),vec_(4000),
            op_(vec_.begin()), ed_(vec_.end()), cur_(op_), data_op_(op_)
        {

        }

        Ring_buffer(std::size_t max_size__)
        :max_size_(max_size__), size_(0)
    	, mutex_(),vec_(max_size__),op_(vec_.begin()), ed_(vec_.end()), cur_(op_), data_op_(op_)
        {

        }

        ~Ring_buffer(){}


        std::size_t size()
        {
            std::lock_guard<std::mutex> lock_(mutex_);
            return size_;
        }

        std::size_t max_size()
        {
            std::lock_guard<std::mutex> lock_(mutex_);
            return max_size_;
        }


        bool empty()
        {
            std::lock_guard<std::mutex> lock_(mutex_);
            return size_ == 0;
        }


        std::size_t left()
        {
            std::lock_guard<std::mutex> lock_(mutex_);
            return max_size_ - size_;
        }



        template<typename InputIt>
        std::size_t read(InputIt first, int size__)
        {
            std::lock_guard<std::mutex> lock_(mutex_);

            int readlen = left();
            if(size__ < readlen)
            {
                readlen = size__;
            }

            std::ptrdiff_t cur_end_len = ed_ -cur_;
            if(cur_end_len < readlen)
            {
                cur_ = std::copy(first, first + readlen, std::inserter(vec_, cur_));
            }
            else
            {
                std::copy(first, first + cur_end_len, std::inserter(vec_, cur_));
                cur_ = std::copy(first + cur_end_len, first + readlen, std::inserter(vec_, vec_.begin()));
            }

            size_ += readlen;

            return readlen;

        }

        template<typename OutputIt>
        std::size_t write(OutputIt first, int size__)
        {
            std::lock_guard<std::mutex> lock_(mutex_);
            int writelen = size__;
            if(writelen > size_)
            {
                writelen = size_;
            }

            std::ptrdiff_t cur_end_len = ed_ -cur_;
            if(cur_end_len < writelen)
            {
                std::copy(data_op_, vec_.end(), first);
                std::copy(vec_.begin(), cur_, first);
                data_op_ = vec_.begin();
            }
            else
            {
                std::copy(data_op_, data_op_ + writelen, first);
                data_op_ += writelen;
            }

            size_ -= writelen;

            return writelen;
        }

        std::size_t read(std::shared_ptr<Socket> socket)
        {
            std::lock_guard<std::mutex> lock_(mutex_);
            int fd = socket->get_fd();
            std::ptrdiff_t cur_end_len = ed_ -cur_;
            int readlen = ::read(fd, &(*cur_), cur_end_len);
            if(readlen < cur_end_len )
            {
                 cur_ += readlen;
                 size_ += readlen;
                 return readlen;
            }

            std::ptrdiff_t op_data_len = data_op_ - op_;

            int tmp_read = ::read(fd, &(*op_), op_data_len);

            cur_ = op_ + tmp_read;
            readlen += tmp_read;
            size_ += readlen;

            return readlen;
        }

        std::size_t write(std::shared_ptr<Socket> socket)
        {
            std::lock_guard<std::mutex> lock_(mutex_);
            if(size_ == 0)
                return 0;
            int writelen;
            int fd = socket->get_fd();
            if(data_op_ < cur_)
            {
                writelen = cur_ - data_op_;
                writelen = ::write(fd, (&(*data_op_)), writelen);
                data_op_ += writelen;
                return writelen;
            }

            std::ptrdiff_t data_end_len = ed_ - data_op_;
            writelen = ::write(fd, (&(*data_op_)), data_end_len);

            while(writelen != data_end_len)
            {
                ::write(fd, &(*(data_op_ + writelen)), data_end_len - writelen);
            }

            std::ptrdiff_t op_cur_len = cur_ - op_;
            int tmp_write = ::write(fd, &(*op_), op_cur_len);

            writelen += tmp_write;
            data_op_ = op_ + tmp_write;

            size_ -= writelen;

            return writelen;

        }

    private:

        typename std::vector<T> vec_;

        typename std::vector<T>::iterator op_;
        typename std::vector<T>::iterator ed_;

        typename std::vector<T>::iterator cur_;
        typename std::vector<T>::iterator data_op_;


        std::size_t max_size_;
        std::size_t size_;

        std::mutex mutex_;

    };
}

#endif

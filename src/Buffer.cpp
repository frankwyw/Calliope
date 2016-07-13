#include "Buffer.h"
#include <unistd.h>


namespace honoka
{

    Buffer::Buffer():vec_(4000), op_(vec_.begin()), ed_(vec_.end()), cur_(op_), max_size_(4000), size_(0)
    {

    }

    Buffer::Buffer(int size)::vec_(size), op_(vec_.begin()), ed_(vec_.end()), cur_(op_), max_size_(4000), size_(0)
    {

    }

    Buffer::Buffer(std::size_t max_size):max_size_(max_size), size_(0), mutex()
    {

    }

    ~Buffer::Buffer()
    {

    }

    std::size_t Buffer::size()
    {
        std::lock_gurad lock_(mutex_);
        return size_;
    }

    std::size_t Buffer::max_size()
    {
        std::lock_gurad lock_(mutex_);
        return max_size_;
    }

    std::size_t left()
    {
        std::lock_gurad lock_(mutex_);
        return max_size_ - size_;
    }

    virual bool Buffer::empty()
    {
        std::lock_gurad lock_(mutex_);
        return size == 0;
    }

    //必须一次性读完
    template<typename InputIt>
    virtual void Buffer::read(InputIt first, InputIt last, int size__);

    template<typename OutputIt>
    virtual void Buffer::write(OutputIt first, int size__);


    //Ring_buffer

    template <typename T>
    Ring_buffer<T>::Ring_buffer():Buffer(4000),vec_(4000),
        op_(vec_.begin()), ed_(vec_.end()), cur_(op_), data_op_(op_)
    {

    }

    template <typename T>
    Ring_buffer<T>::Ring_buffer(std::size_t size__):Buffer(size__),vec_(size__),
        op_(vec_.begin()), ed_(vec_.end()), cur_(op_), data_op_(op_)
    {

    }

    template <typename T>
    ~Ring_buffer<T>::Ring_buffer()
    {

    }

    template <typename T>
    template<typename InputIt>
    virtual std::size_t Ring_buffer<T>::read(InputIt first, int size__)
    {
        std::lock_gurad lock_(mutex_);

        int readlen = left();
        if(size__ < readlen)
        {
            readlen == size__;
        }

        std::ptrdiff_t cur_end_len = ed_ -cur_;
        if(cur_end_len < readlen)
        {
            cur_ = std::copy(first, first + readlen, std::inserter(vec, cur_));
        }
        else
        {
            std::copy(first, first + cur_end_len, std::inserter(vec, cur_));
            cur_ = std::copy(first + cur_end_len, first + readlen, std::inserter(vec, vec.begin()));
        }

        size_ += readlen;

        return readlen;

    }


    template <typename T>
    template<typename OutputIt>
    virtual std::size_t Ring_buffer<T>::write(OutputIt first, int size__)
    {
        std::lock_gurad lock_(mutex_);
        int writelen = size__;
        if(writelen > size_)
        {
            writelen = size_;
        }

        std::ptrdiff_t cur_end_len = ed_ -cur_;
        if(cur_end_len < writelen)
        {
            std::copy(data_op_, vec.end(), std::inserter(vec, cur_));
            data_op_ = std::copy(vec.begin(), cur_, std::inserter(vec, vec.begin()));
        }
        else
        {
            data_op_ = std::copy(data_op_, data_op_ + writelen, std::inserter(vec, data_op_));
        }

        size_ -= writelen;

        return writelen;
    }

    virtual std::size_t read(std::shared_ptr<Socket> socket)
    {
        int fd = socket->get_fd();
        std::ptrdiff_t cur_end_len = ed_ -cur_;
        int readlen = ::read(fd, cur_->get(), cur_end_len);
        if(readlen < cur_end_len )
        {
             cur_ += readlen;
             return readlen;
        }

        std::ptrdiff_t op_data_len = data_op_ - op_;

        int tmp_read = ::read(fd, op_->get(), op_data_len);

        cur_ = op_ + tmp_read;
        readlen += tmp_read;
        size_ += readlen;

        return readlen;
    }

    virtual std::size_t write(std::shared_ptr<Socket> socket)
    {
        int writelen;
        int fd = socket->get_fd();
        if(data_op_ < cur_)
        {
            writelen = cur_ - data_op_;
            writelen = ::write(fd, data_op_->get(), writelen);
            data_op_ = cur_ + writelen;
            return writelen;
        }

        std::ptrdiff_t data_end_len = ed_ - data_op_;
        writelen = ::write(fd, data_op_->get(), data_end_len);

        while(writelen != data_end_len)
        {
            ::write(fd, (data_op_ + writelen)->get(), data_end_len - writelen);
        }

        std::ptrdiff_t op_cur_len = cur_ - op_;
        int tmp_write = ::write(fd, op_->get(), op_cur_len);

        write += tmp_write;
        data_op_ = op + tmp_write;

        size_ -= wrilen;

        return writelen;

    }


}

#ifndef __BUFFER__H
#define __BUFFER__H

#include <vector>
#include <mutex>
#include <memory>


namespace honoka
{
    class Socket;

    class Buffer
    {
    public:
        Buffer(std::size_t max_size_);

        virtual ~Buffer();

        template<typename InputIt>
        std::size_t read(InputIt first, int size);

        template<typename OutputIt>
        std::size_t write(OutputIt first, int size__);

        virtual std::size_t read(std::shared_ptr<Socket> socket) = 0;
        virtual std::size_t write(std::shared_ptr<Socket> socket) = 0;

        std::size_t size();
        std::size_t max_size();

        bool empty();
        std::size_t left();
    protected:
        std::size_t max_size_;
        std::size_t size_;

        std::mutex mutex_;
    };

    template<typename T>
    class Ring_buffer : public Buffer
    {
    public:

        Ring_buffer();
        Ring_buffer(std::size_t size__);
        virtual ~Ring_buffer();

        template<typename InputIt>
        std::size_t read(InputIt first, int size);

        template<typename OutputIt>
        std::size_t write(OutputIt first, int size__);

        std::size_t read(std::shared_ptr<Socket> socket);
        std::size_t write(std::shared_ptr<Socket> socket);

    private:

        typename std::vector<T> vec_;

        typename std::vector<T>::iterator op_;
        typename std::vector<T>::iterator ed_;

        typename std::vector<T>::iterator cur_;
        typename std::vector<T>::iterator data_op_;

    };
}

#endif

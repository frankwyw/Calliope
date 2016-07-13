#ifndef __BUFFER__H
#define __BUFFER__H

#include <vector>
#include <mutex>


namespace honoka
{
    class Buffer
    {
    public:
        Buffer(std::size_t max_size_);

        ~Buffer();

        template<typename InputIt>
        virtual std::size_t read(InputIt first, int size) = 0;

        template<typename OutputIt>
        virtual std::size_t write(OutputIt first, int size__) = 0;

        virtual std::size_t read(std::shared_ptr<Socket> socket) = 0;
        virtual std::size_t write(std::shared_ptr<Socket> socket) = 0;

        std::size_t size();
        std::size_t max_size();

        virual bool empty();
        std::size_t left();
    private:
        std::size_t max_size_;
        std::size_t size_;

        std::mutex mutex_;
    };

    template<typename T>
    class Ring_buffer : public Buffer
    {
    public:

        Ring_buffer();
        Ring_buffer(int size);
        ~Ring_buffer();

        template<typename InputIt>
        virtual std::size_t read(InputIt first, int size);

        template<typename OutputIt>
        virtual std::size_t write(OutputIt first, int size__);

        virtual std::size_t read(std::shared_ptr<Socket> socket);
        virtual std::size_t write(std::shared_ptr<Socket> socket);

        virual bool empty();
    private:

        std::vector<T> vec;

        std::vector<T>::iterator op_;
        std::vector<T>::iterator ed_;

        std::vector<T>::iterator cur_;
        std::vector<T>::iterator data_op_;

    };
}

#endif

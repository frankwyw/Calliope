
CXX = g++

HONOKA_DIR = ../..

HONOKA_SRCS = ${HONOKA_DIR}/src

HONOKA_INCLUDES = ${HONOKA_DIR}/include

HONOKA_TEST_DIR = ..

CPPFLAGS += -isystem ${HONOKA_TEST_DIR}/gtest/include -I${HONOKA_DIR}/include

CXXFLAGS += -g -Wall -Wextra -pthread -std=c++11 -Igtest/include -I${HONOKA_DIR}/src

TESTS = unit_tests

LINKFLAGS = -lpthread -L${HONOKA_TEST_DIR}/gtest -lgtest -lgtest_main



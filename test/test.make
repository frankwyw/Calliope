
CXX = g++

HONOKA_DIR = ../..

HONOKA_SRCS = ${HONOKA_DIR}/src

HONOKA_TEST_DIR = ..

CPPFLAGS += -isystem ${HONOKA_TEST_DIR}/gtest/include

CXXFLAGS += -g -Wall -Wextra -pthread -std=c++11 -I../gtest/include

TESTS = unit_tests

LINKFLAGS = -lpthread -L../gtest/lib -lgtest -lgtest_main



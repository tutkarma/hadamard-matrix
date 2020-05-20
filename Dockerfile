FROM gcc:9.3.0

RUN apt-get update && \
    apt-get install -y \
    check cmake valgrind

ADD CMakeLists.txt /tmp
ADD cmake /tmp/cmake
ADD data /tmp/data
ADD equivalence /tmp/equivalence
ADD tests /tmp/tests

WORKDIR /tmp

RUN cmake . && \
    cmake --build . && \
    CTEST_OUTPUT_ON_FAILURE=TRUE cmake --build . --target test

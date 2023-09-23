FROM gcc:13.2
WORKDIR /usr/local/rinha-compiladores

COPY Makefile .
COPY src src
COPY include include
COPY lib lib

RUN make clean all

CMD ["./rinha-compiladores", "/var/rinha/source.rinha.json"]

FROM bellsoft/alpaquita-linux-gcc
WORKDIR /usr/local/rinha-compiladores
COPY ./rinha-compiladores ./

CMD ["time", "./rinha-compiladores", "/var/rinha/source.rinha.json"]

FROM gcc:13.2
WORKDIR /usr/local/rinha-compiladores

#RUN apt-get update && apt-get -y install time

COPY ./rinha-compiladores ./

#CMD ["/usr/bin/time", "-p", "./rinha-compiladores", "/var/rinha/source.rinha.json"]
CMD ["./rinha-compiladores", "/var/rinha/source.rinha.json"]

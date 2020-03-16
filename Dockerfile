FROM python:3.7.7-stretch	
RUN apt-get update
WORKDIR /home/sebastian/Desktop/docker
EXPOSE 8888/udp
COPY ./script.py .
CMD [ "python", "./script.py" ]
#docker build -t "miimagen" .
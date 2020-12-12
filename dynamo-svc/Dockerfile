FROM alpine:edge

# Update and install dependencies
RUN apk add --no-cache --repository http://dl-3.alpinelinux.org/alpine/edge/testing/ git binutils boost-dev build-base cmake gcc make curl-dev libcurl openssl-dev ossp-uuid-dev zlib-dev

# Clone, build and install aws-skd-cpp
RUN git clone https://github.com/aws/aws-sdk-cpp.git /usr/vts_work/aws-sdk-cpp && \
	mkdir /usr/vts_work/aws-sdk-cpp/build && cd /usr/vts_work/aws-sdk-cpp/build && \
	cmake -DBUILD_ONLY="dynamodb;s3;sqs" -DENABLE_TESTING=OFF -DCUSTOM_MEMORY_MANAGEMENT=OFF -DBUILD_SHARED_LIBS=OFF /usr/vts_work/aws-sdk-cpp && \
    make &&  make install && \
	rm -rf /usr/vts_work/aws-sdk-cpp
		
# Clone, build and install httplib
RUN git clone https://github.com/yhirose/cpp-httplib.git /usr/vts_work/httplib && \
	mkdir /usr/vts_work/httplib/build && cd /usr/vts_work/httplib/build && \
	cmake /usr/vts_work/httplib && \
	make &&  make install && rm -rf /usr/vts_work/httplib

# Clone, build and install json
RUN git clone https://github.com/nlohmann/json.git /usr/vts_work/json && \
	mkdir /usr/vts_work/json/build && cd /usr/vts_work/json/build && \
	cmake -DJSON_BuildTests=OFF /usr/vts_work/json && \
	make &&  make install && rm -rf /usr/vts_work/json


RUN mkdir /bin_vitanza
	
RUN git clone https://github.com/isil-tech-classroom/examen-final-dominique120.git /usr/vts_work/ef && \
	mkdir /usr/vts_work/ef/build && cd /usr/vts_work/ef/build && \
	cmake -DDB_DYNAMO=ON /usr/vts_work/ef/dynamo-svc && \
	make && \
	mv /usr/vts_work/ef/build/dynamo-svc /bin_vitanza/dynamo-svc && \
	cp /usr/vts_work/ef/dynamo-svc/config.json /bin_vitanza/config.json && \
	rm -rf /usr/vts_work/ef	
	
EXPOSE 80
WORKDIR /bin_vitanza
VOLUME /bin_vitanza
ENTRYPOINT /bin_vitanza/dynamo-svc 0.0.0.0 80

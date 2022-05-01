
# Download Ubuntu Document 
	FROM ubuntu

# Update packages
	RUN apt-get update && apt-get install -y bzip2 curl

#Install Commands
RUN curl -L -o ~/miniconda.sh -O  https://repo.continuum.io/miniconda/Miniconda3-latest-Linuxx86_64.sh  && \
	chmod +x ~/miniconda.sh && \
	~/miniconda.sh -b -p /opt/conda && \
	rm ~/miniconda.sh && \
	/opt/conda/bin/conda clean -ya
	
ENV PATH /opt/conda/bin:$PATH
RUN conda init zsh
RUN conda create -n CS_321
RUN conda UPDATE -n base -c defaults conda
RUN conda install -c anaconda jupyter 


	
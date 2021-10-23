$ java -jar "-Djava.library.path=." "-Daida.pva.channels.filename=channels.yaml" ../../lib/aida-pva.jar

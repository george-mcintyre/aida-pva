$! Rebuild and rerun AIDA-PVA provider
$ cd [--.SRC.BUILD.SLC]
$ @BUILD
$ cd [---.lib.SLC]
$ java -jar "-Djava.library.path=." "-Daida.pva.channels.filename=channels.yaml" ../aida-pva.jar

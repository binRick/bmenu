make clean;passh -L .make make  && env CONFIG_FILE=./c1 ./bin/test1 2>.e; echo $? && cat .e

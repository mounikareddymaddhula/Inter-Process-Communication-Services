1. Build the source using make command. Make builds everything. 
2. To build server, use 'make server'. 
3. To build client, use 'make client.o'
4. To build the application, use 'make app1'
5. The server can be run as follows:
   - ./server [shared memory size]
6. The application can be run as follows:
   - ./app1 list_of_file_names_seperated_by_space
7. The application has the code to test both synchronous and asynchronous communication
8. Currently, the code to test synchronous is enabled and the code to test asynchronous is disabled. This can be changed using the macro ASYNC.
9. the output will be present in the same folder and it is named as follows:
   output_{num}sync.txt in case of synchronous where num is the cardinal number of the file in the set of arguments (starting at 1)
   output_{num}.txt in case of asynchronous where num is the cardinal number of the file in the set of arguments (starting at 1)
10. Multiple clients can be realized by running multiple parallel instances of app1.

C:\Users\son\vcpkg\installed\x64-windows\tools\protobuf\protoc.exe -I=. --cpp_out=. message.proto

copy message.pb.h  ..\Server\DTO\message.pb.h
copy message.pb.cc  ..\Server\DTO\message.pb.cc

pause

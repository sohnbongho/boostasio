C:\Users\son\vcpkg\installed\x64-windows\tools\protobuf\protoc.exe -I=. --cpp_out=. message.proto

copy message.pb.h  ..\DummyClient\DTO\message.pb.h
copy message.pb.cc  ..\DummyClient\DTO\message.pb.cc

pause

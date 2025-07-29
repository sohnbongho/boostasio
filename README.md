# 🚀 Boost.Asio MMO Server - Protobuf 설정 가이드

Boost.Asio를 이용한 C++ 서버 만들기 
- boost 1.88
- google protobuffer

이 문서는 `vcpkg`를 이용해 Protobuf를 설치하고 C++ 프로젝트에 연동하는 방법을 안내합니다.

---

## 🔧 vcpkg 설치 방법 (처음 한 번만)

```bash
git clone https://github.com/microsoft/vcpkg.git

cd vcpkg
bootstrap-vcpkg.bat

vcpkg install protobuf:x64-windows
.\vcpkg\installed\x64-windows\tools\protobuf\protoc.exe -I=. --cpp_out=. message.proto
# Image Processing Desktop App

## 依赖安装
1. **Python依赖**: `pip install -r requirements.txt`
2. **C++依赖**: Boost.Python, Python开发库
   - Ubuntu: `sudo apt install libboost-python-dev python3-dev`
   - Windows: 使用vcpkg安装 `vcpkg install boost-python`

## 运行步骤
1. 编译后端:
mkdir build && cd build

cmake .. -DCMAKE_TOOLCHAIN_FILE="D:/wechat/vcpkg-master/vcpkg/scripts/buildsystems/vcpkg.cmake" ^ -DBOOST_ROOT="D:/wechat/vcpkg-master/vcpkg/installed/x64-windows" 

cmake --build . --config Release

set PYTHONHOME=D:\wechat\vcpkg-master\vcpkg\packages\python3_x64-windows\tools\python3
set PYTHONPATH=D:\wechat\vcpkg-master\vcpkg\packages\python3_x64-windows\tools\python3\Lib;D:\wechat\vcpkg-master\vcpkg\packages\python3_x64-windows\tools\python3\Lib\site-packages
2. 启动后端: `D:\mk4\build\Release\backend.exe`
3. 启动前端: `python D:\mk4\main.py`
4. 通过界面上传图像并处理。

## 打包说明
- **Windows**: 使用pyinstaller打包，包含Python环境和后端可执行文件。
pyinstaller -F main.py --add-binary "..\backend\build\Release\backend.exe;." --distpath "..\dist"


- **Linux**: 使用CPack生成deb包，确保Python依赖可用。

## 可执行文件生成
编译后端：cmake --build backend/build --target backend
使用PyInstaller打包前端：pyinstaller -F frontend/main.py --add-binary backend/build/backend:./  
最终输出在dist/目录下。
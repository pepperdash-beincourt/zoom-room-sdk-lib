#!/usr/bin/env python3
"""
Zoom Rooms Controller SDK - Cross-Platform Setup Script
Supports: macOS, Linux, Windows
Usage: python setup.py [command]
"""

import os
import sys
import platform
import subprocess
import shutil
from pathlib import Path

# Color codes for terminal output
class Colors:
    if platform.system() != 'Windows':
        RED = '\033[0;31m'
        GREEN = '\033[0;32m'
        YELLOW = '\033[1;33m'
        BLUE = '\033[0;34m'
        CYAN = '\033[0;36m'
        NC = '\033[0m'
    else:
        # Windows doesn't support ANSI colors by default in older terminals
        RED = GREEN = YELLOW = BLUE = CYAN = NC = ''

class Logger:
    @staticmethod
    def info(msg):
        print(f"{Colors.BLUE}[INFO]{Colors.NC} {msg}")
    
    @staticmethod
    def success(msg):
        print(f"{Colors.GREEN}[✓]{Colors.NC} {msg}")
    
    @staticmethod
    def warning(msg):
        print(f"{Colors.YELLOW}[!]{Colors.NC} {msg}")
    
    @staticmethod
    def error(msg):
        print(f"{Colors.RED}[✗]{Colors.NC} {msg}")
    
    @staticmethod
    def step(msg):
        print(f"{Colors.CYAN}[STEP]{Colors.NC} {msg}")

    @staticmethod
    def separator():
        print("=" * 64)

class ZRCSDKSetup:
    def __init__(self):
        # Script is in Demo directory
        self.script_dir = Path(__file__).parent.resolve()
        self.demo_dir = self.script_dir  # Current directory is Demo
        self.zrcsdk_root = self.script_dir.parent  # Parent directory is SDK root
        self.zrcsdk_include = self.zrcsdk_root / "include"
        self.zrcsdk_libs = self.zrcsdk_root / "libs"
        self.build_dir = self.demo_dir / "build"
        
        self.platform = platform.system()  # 'Darwin', 'Linux', 'Windows'
        self.arch = platform.machine()  # 'x86_64', 'arm64', 'AMD64', etc.
        
    def command_exists(self, cmd):
        """Check if a command exists"""
        return shutil.which(cmd) is not None
    
    def run_command(self, cmd, cwd=None, shell=False, check=True, show_output=False):
        """Run a shell command"""
        try:
            result = subprocess.run(
                cmd,
                cwd=cwd,
                shell=shell,
                check=check,
                capture_output=True,
                text=True
            )
            if show_output and result.returncode != 0:
                if result.stdout:
                    print(result.stdout)
                if result.stderr:
                    print(result.stderr)
            return result.returncode == 0
        except subprocess.CalledProcessError as e:
            if show_output:
                if e.stdout:
                    print(e.stdout)
                if e.stderr:
                    print(e.stderr)
            return False
    
    def show_welcome(self):
        """Show welcome message"""
        os.system('cls' if self.platform == 'Windows' else 'clear')
        Logger.separator()
        print(f"{Colors.GREEN}    Zoom Rooms Controller SDK - Cross-Platform Setup{Colors.NC}")
        Logger.separator()
        print()
        print("This script will automatically:")
        print("  ✓ Check system environment and development tools")
        print("  ✓ Verify SDK structure")
        print("  ✓ Setup environment variables")
        print("  ✓ Generate build project")
        print("  ✓ Build Demo application")
        print()
        Logger.separator()
        print()
    
    def check_environment(self):
        """Check system environment"""
        Logger.step("Step 1/5: Checking system environment...")
        
        Logger.success(f"Operating System: {self.platform}")
        Logger.info(f"CPU Architecture: {self.arch}")
        
        if self.platform == "Darwin":
            version = platform.mac_ver()[0]
            Logger.info(f"macOS Version: {version}")
            if self.arch == "arm64":
                Logger.success("Detected Apple Silicon")
            else:
                Logger.error("Only Apple Silicon is supported for macOS")
                Logger.info("Intel Macs are not supported")
                sys.exit(1)
        elif self.platform == "Linux":
            try:
                import distro
                Logger.info(f"Distribution: {distro.name()} {distro.version()}")
            except ImportError:
                Logger.info("Linux distribution")
        elif self.platform == "Windows":
            Logger.info(f"Windows Version: {platform.version()}")
        
        print()
    
    def check_tools(self):
        """Check development tools"""
        Logger.step("Step 2/5: Checking development tools...")
        
        # Check CMake (required for all platforms)
        if not self.command_exists("cmake"):
            Logger.error("CMake not installed")
            if self.platform == "Darwin":
                Logger.info("Install: brew install cmake")
            elif self.platform == "Linux":
                Logger.info("Install: sudo apt install cmake  # Debian/Ubuntu")
                Logger.info("        sudo yum install cmake  # RHEL/CentOS")
            elif self.platform == "Windows":
                Logger.info("Download from: https://cmake.org/download/")
            sys.exit(1)
        
        # Get CMake version
        result = subprocess.run(["cmake", "--version"], capture_output=True, text=True)
        cmake_version = result.stdout.split('\n')[0]
        Logger.success(f"CMake: {cmake_version}")
        
        # Platform-specific tools
        if self.platform == "Darwin":
            # Check Xcode Command Line Tools
            if not self.command_exists("xcodebuild"):
                Logger.error("Xcode Command Line Tools not installed")
                Logger.info("Please run: xcode-select --install")
                sys.exit(1)
            result = subprocess.run(["xcodebuild", "-version"], capture_output=True, text=True)
            xcode_version = result.stdout.split('\n')[0]
            Logger.success(f"Xcode: {xcode_version}")
        
        elif self.platform == "Linux":
            # Check GCC/G++
            if not self.command_exists("g++"):
                Logger.error("G++ compiler not installed")
                Logger.info("Install: sudo apt install build-essential  # Debian/Ubuntu")
                sys.exit(1)
            result = subprocess.run(["g++", "--version"], capture_output=True, text=True)
            gcc_version = result.stdout.split('\n')[0]
            Logger.success(f"Compiler: {gcc_version}")
        
        elif self.platform == "Windows":
            # Check for Visual Studio or other build tools
            # Note: CMake can detect Visual Studio automatically
            Logger.info("Checking for Visual Studio...")
            if not self.run_command(["cmake", "--help"], check=False):
                Logger.error("CMake not properly configured")
                sys.exit(1)
            Logger.success("Build tools available")
        
        print()
    
    def verify_sdk(self):
        """Verify SDK structure"""
        Logger.step("Step 3/5: Verifying SDK structure...")
        
        missing_files = []
        
        # Check critical directories and files
        if not self.zrcsdk_include.is_dir():
            missing_files.append("include/")
        if not self.zrcsdk_libs.is_dir():
            missing_files.append("libs/")
        if not self.demo_dir.is_dir():
            missing_files.append("Demo/")
        if not (self.zrcsdk_include / "IZRCSDK.h").is_file():
            missing_files.append("include/IZRCSDK.h")
        if not (self.demo_dir / "CMakeLists.txt").is_file():
            missing_files.append("Demo/CMakeLists.txt")
        
        # Platform-specific SDK files
        if self.platform == "Darwin":
            if not (self.zrcsdk_libs / "ZRCSdk.framework").is_dir():
                missing_files.append("libs/ZRCSdk.framework")
        elif self.platform == "Linux":
            # Check for .so files
            so_files = list(self.zrcsdk_libs.glob("*.so"))
            if not so_files and not (self.zrcsdk_libs / "libZRCSdk.so").is_file():
                Logger.warning("No .so files found in libs/")
        elif self.platform == "Windows":
            # Check for .lib files
            lib_files = list(self.zrcsdk_libs.glob("*.lib"))
            if not lib_files and not (self.zrcsdk_libs / "ZRCSdk.lib").is_file():
                Logger.warning("No .lib files found in libs/")
        
        if missing_files:
            Logger.error("SDK structure incomplete. Missing files/directories:")
            for file in missing_files:
                print(f"  - {file}")
            sys.exit(1)
        
        Logger.success("SDK structure verified")
        Logger.info(f"SDK Root: {self.zrcsdk_root}")
        Logger.info(f"Include: {self.zrcsdk_include}")
        Logger.info(f"Libraries: {self.zrcsdk_libs}")
        Logger.info(f"Demo: {self.demo_dir}")
        
        # Copy SDK files to Demo directories
        Logger.info("Copying SDK files to Demo directories...")
        self._copy_sdk_files()
        
        print()
    
    def _copy_sdk_files(self):
        """Copy SDK include and libs to Demo directories"""
        demo_include_sdk = self.demo_dir / "include" / "include"
        demo_libs = self.demo_dir / "libs"
        
        # Copy include files to Demo/include/include/
        if demo_include_sdk.exists():
            shutil.rmtree(demo_include_sdk)
        demo_include_sdk.parent.mkdir(parents=True, exist_ok=True)
        shutil.copytree(self.zrcsdk_include, demo_include_sdk)
        Logger.success(f"Copied SDK headers to {demo_include_sdk}")
        
        # Copy libs to Demo/libs/
        if demo_libs.exists():
            shutil.rmtree(demo_libs)
        demo_libs.mkdir(parents=True, exist_ok=True)
        
        # Copy all files from SDK libs
        for item in self.zrcsdk_libs.iterdir():
            dest = demo_libs / item.name
            if item.is_dir():
                shutil.copytree(item, dest)
            else:
                shutil.copy2(item, dest)
        
        Logger.success(f"Copied SDK libraries to {demo_libs}")
        
        # Windows: Also note the dll directory location (don't copy here, will copy at build/run time)
        if self.platform == "Windows":
            sdk_dll_dir = self.zrcsdk_root / "dll"
            if sdk_dll_dir.exists():
                dll_count = len(list(sdk_dll_dir.glob("*.dll")))
                Logger.info(f"Found {dll_count} DLL file(s) in {sdk_dll_dir} (will copy at build time)")
    
    def setup_libuv_linux(self):
        """Setup libuv for Linux (required for Demo)"""
        if self.platform != "Linux":
            return
        
        Logger.step("Step 3.5/5: Setting up libuv for Linux...")
        
        demo_libs_dir = self.demo_dir / "libs"
        demo_include_libuv = self.demo_dir / "include" / "libuv" / "include"
        demo_libuv_a = demo_libs_dir / "libuv.a"
        
        # Check if libuv is already in Demo (partial or complete)
        has_lib = demo_libuv_a.exists()
        has_headers = demo_include_libuv.exists() and any(demo_include_libuv.glob("uv*.h"))
        
        if has_lib and has_headers:
            Logger.success("libuv already configured in Demo")
            return
        elif has_lib or has_headers:
            # Partial setup - warn but continue if system has libuv
            Logger.warning("Partial libuv setup detected, will complete missing parts")
        
        # Check system libuv installation (only if needed)
        needs_lib = not has_lib
        needs_headers = not has_headers
        
        if needs_lib or needs_headers:
            system_libuv_static = Path("/usr/lib/x86_64-linux-gnu/libuv_a.a")
            system_libuv_header = Path("/usr/include/uv.h")
            
            # Only error if we need something and system doesn't have it
            if (needs_lib and not system_libuv_static.exists()) or \
               (needs_headers and not system_libuv_header.exists()):
                Logger.error("libuv-dev not installed on system")
                Logger.info("Please install libuv development package:")
                Logger.info("  sudo apt-get install libuv1-dev  # Ubuntu/Debian")
                Logger.info("  sudo yum install libuv-devel     # RHEL/CentOS")
                sys.exit(1)
        
        # Create directories
        if needs_lib:
            demo_libs_dir.mkdir(parents=True, exist_ok=True)
        if needs_headers:
            demo_include_libuv.mkdir(parents=True, exist_ok=True)
        
        # Copy libuv static library (only if needed)
        if needs_lib:
            Logger.info(f"Copying {system_libuv_static} to {demo_libuv_a}")
            shutil.copy2(system_libuv_static, demo_libuv_a)
        
        # Copy libuv headers (only if needed)
        if needs_headers:
            Logger.info("Copying libuv headers...")
            import glob
            for header in glob.glob("/usr/include/uv*.h"):
                shutil.copy2(header, demo_include_libuv)
            
            # Copy uv directory if exists
            system_uv_dir = Path("/usr/include/uv")
            if system_uv_dir.exists():
                demo_uv_dir = demo_include_libuv / "uv"
                if demo_uv_dir.exists():
                    shutil.rmtree(demo_uv_dir)
                shutil.copytree(system_uv_dir, demo_uv_dir)
        
        Logger.success("libuv configured successfully")
        Logger.info(f"  Static library: {demo_libuv_a}")
        Logger.info(f"  Headers: {demo_include_libuv}")
        print()
    
    def setup_environment(self):
        """Setup environment variables"""
        Logger.step("Step 4/5: Setting up environment variables...")
        
        if self.platform == "Windows":
            env_file = self.zrcsdk_root / "zrcsdk_env.bat"
            content = f"""@echo off
REM Zoom Rooms Controller SDK Environment Variables
REM Usage: zrcsdk_env.bat

set ZRCSDK_ROOT={self.zrcsdk_root}
set ZRCSDK_INCLUDE=%ZRCSDK_ROOT%\\include
set ZRCSDK_LIBS=%ZRCSDK_ROOT%\\libs
set PATH=%ZRCSDK_LIBS%;%PATH%

echo [OK] ZRCSDK environment variables loaded
echo   SDK Root: %ZRCSDK_ROOT%
"""
        else:
            env_file = self.zrcsdk_root / "zrcsdk_env.sh"
            ld_var = "DYLD_LIBRARY_PATH" if self.platform == "Darwin" else "LD_LIBRARY_PATH"
            content = f"""#!/bin/bash
# Zoom Rooms Controller SDK Environment Variables
# Usage: source zrcsdk_env.sh

SCRIPT_DIR="$(cd "$(dirname "${{BASH_SOURCE[0]}}")" && pwd)"
export ZRCSDK_ROOT="${{SCRIPT_DIR}}"
export ZRCSDK_INCLUDE="${{ZRCSDK_ROOT}}/include"
export ZRCSDK_LIBS="${{ZRCSDK_ROOT}}/libs"
export {ld_var}="${{ZRCSDK_LIBS}}:${{{ld_var}}}"
export CMAKE_PREFIX_PATH="${{ZRCSDK_ROOT}}:${{CMAKE_PREFIX_PATH}}"
"""
            if self.platform == "Darwin":
                content += 'export DYLD_FRAMEWORK_PATH="${ZRCSDK_LIBS}:${DYLD_FRAMEWORK_PATH}"\n'
            
            content += '\necho "✓ ZRCSDK environment variables loaded"\n'
            content += 'echo "  SDK Root: ${ZRCSDK_ROOT}"\n'
        
        with open(env_file, 'w') as f:
            f.write(content)
        
        if self.platform != "Windows":
            os.chmod(env_file, 0o755)
        
        Logger.success("Environment variables configured")
        Logger.info(f"Environment file: {env_file}")
        print()
    
    def generate_and_build(self):
        """Generate build project and build"""
        Logger.step("Step 5/5: Generating build project and building...")
        
        os.chdir(self.demo_dir)
        
        # Handle existing build directory
        if self.build_dir.exists():
            # On Windows, don't force delete - just reconfigure
            if self.platform == "Windows":
                Logger.info("Build directory exists, will reconfigure...")
            else:
                # On macOS/Linux, try to clean
                try:
                    Logger.info("Cleaning old build directory...")
                    shutil.rmtree(self.build_dir)
                except Exception as e:
                    Logger.warning(f"Could not remove build directory: {e}")
                    Logger.info("Will try to reconfigure instead...")
        
        self.build_dir.mkdir(exist_ok=True)
        
        # Configure CMake
        Logger.info(f"Running CMake (architecture: {self.arch})...")
        
        cmake_args = ["cmake", "-B", "build"]
        
        if self.platform == "Darwin":
            cmake_args.extend([
                "-G", "Xcode",
                "-DCMAKE_OSX_ARCHITECTURES=arm64",  # Only Apple Silicon supported
                "-DCMAKE_BUILD_TYPE=Release",
                "-DCMAKE_OSX_DEPLOYMENT_TARGET=10.15"
            ])
        elif self.platform == "Linux":
            cmake_args.extend([
                "-DCMAKE_BUILD_TYPE=Release"
            ])
        elif self.platform == "Windows":
            # Let CMake auto-detect Visual Studio version
            cmake_args.extend([
                "-A", "x64"
            ])
        
        if not self.run_command(cmake_args, cwd=self.demo_dir):
            Logger.error("CMake configuration failed")
            Logger.info(f"Try manually: cd {self.demo_dir} && cmake -B build")
            sys.exit(1)
        
        Logger.success("Build project generated")
        
        if self.platform == "Darwin":
            Logger.info(f"Xcode project: {self.build_dir}/Demo.xcodeproj")
        elif self.platform == "Windows":
            Logger.info(f"Visual Studio solution: {self.build_dir}/Demo.sln")
        
        # Build
        Logger.info("Building Demo application...")
        build_cmd = ["cmake", "--build", "build", "--config", "Release"]
        
        if not self.run_command(build_cmd, cwd=self.demo_dir, show_output=True):
            Logger.error("Build failed")
            Logger.info(f"Try manually: cd {self.demo_dir} && cmake --build build --config Release")
            sys.exit(1)
        
        Logger.success("Build successful!")
        
        # Find executable
        if self.platform == "Windows":
            exe_name = "Demo.exe"
        else:
            exe_name = "Demo"
        
        executable = self._find_executable(exe_name)
        if executable:
            Logger.info(f"Executable: {executable}")
        
        # Windows-specific: Copy DLL files after build
        if self.platform == "Windows" and executable:
            print()
            Logger.info("Copying DLL files to executable directory...")
            Logger.info(f"Target directory: {executable.parent}")
            print()
            
            dll_copied = False
            
            # Windows SDK structure: dll/ folder is at same level as Demo/
            sdk_dll_dir = self.zrcsdk_root / "dll"
            Logger.info(f"Searching: {sdk_dll_dir}")
            
            if sdk_dll_dir.exists():
                dll_files = list(sdk_dll_dir.glob("*.dll"))
                Logger.info(f"  Found {len(dll_files)} DLL file(s)")
                
                if dll_files:
                    for dll in dll_files:
                        try:
                            shutil.copy2(dll, executable.parent)
                            Logger.success(f"  ✓ {dll.name}")
                            dll_copied = True
                        except Exception as e:
                            Logger.warning(f"  ✗ {dll.name}: {e}")
                else:
                    Logger.warning("  No DLL files found in dll directory")
            else:
                Logger.error(f"  DLL directory not found: {sdk_dll_dir}")
            
            print()
            if dll_copied:
                Logger.success(f"DLL files copied to: {executable.parent}")
            else:
                Logger.error("Failed to copy DLL files!")
                print()
                print(f"{Colors.YELLOW}Please manually copy DLL files:{Colors.NC}")
                print(f"  From: {sdk_dll_dir}")
                print(f"  To:   {executable.parent}")
            
            print()
            Logger.info("⚠️  For best development experience on Windows:")
            print(f"   Open the solution in Visual Studio:")
            print(f"   {Colors.CYAN}start {self.build_dir}\\Demo.sln{Colors.NC}")
            if dll_copied:
                print()
                print(f"   DLL files have been copied, you can now run directly in Visual Studio.")
        
        print()
    
    def _find_executable(self, name):
        """Find executable in build directory"""
        for root, dirs, files in os.walk(self.build_dir):
            if name in files:
                return Path(root) / name
        return None
    
    def show_completion(self):
        """Show completion message"""
        Logger.separator()
        Logger.success("🎉 Setup Complete!")
        Logger.separator()
        print()
        print(f"{Colors.GREEN}Usage:{Colors.NC}")
        print()
        print(f"  {Colors.CYAN}1. Run Demo:{Colors.NC}")
        if self.platform == "Windows":
            print(f"     python setup.py run")
        else:
            print(f"     python3 setup.py run")
        print()
        print(f"  {Colors.CYAN}2. Rebuild:{Colors.NC}")
        if self.platform == "Windows":
            print(f"     python setup.py build")
        else:
            print(f"     python3 setup.py build")
        print()
        print(f"  {Colors.CYAN}3. Clean:{Colors.NC}")
        if self.platform == "Windows":
            print(f"     python setup.py clean")
        else:
            print(f"     python3 setup.py clean")
        print()
        
        if self.platform == "Darwin":
            print(f"  {Colors.CYAN}4. Open in Xcode:{Colors.NC}")
            print(f"     open {self.build_dir}/Demo.xcodeproj")
            print()
        elif self.platform == "Windows":
            print(f"  {Colors.CYAN}4. Open in Visual Studio:{Colors.NC}")
            print(f"     start {self.build_dir}\\Demo.sln")
            print()
        
        print(f"{Colors.GREEN}Demo Commands:{Colors.NC}")
        print("  pair <code>    - Pair with Zoom Room")
        print("  startmeeting   - Start meeting")
        print("  mute           - Mute audio")
        print("  endmeeting     - End meeting")
        print("  unpair         - Unpair")
        print("  quit           - Exit")
        print()
        print(f"{Colors.BLUE}Documentation:{Colors.NC} https://developers.zoom.us/docs/zoom-rooms/")
        print()
        Logger.separator()
        
        # Ask to run or open solution
        try:
            if self.platform == "Windows":
                response = input(f"{Colors.CYAN}Open Visual Studio solution? [Y/n]{Colors.NC} ").strip().lower()
                if response != 'n':
                    sln_path = self.build_dir / "Demo.sln"
                    if sln_path.exists():
                        Logger.info(f"Opening {sln_path}...")
                        os.system(f'start "" "{sln_path}"')
                        Logger.success("Visual Studio solution opened!")
                        print()
                        Logger.info("DLL files have been copied to build\\Release\\")
                        Logger.info("You can now run/debug directly in Visual Studio.")
                    else:
                        Logger.error(f"Solution file not found: {sln_path}")
            else:
                response = input(f"{Colors.CYAN}Run Demo now? [Y/n]{Colors.NC} ").strip().lower()
                if response != 'n':
                    self.run_demo()
        except KeyboardInterrupt:
            print()
            pass
    
    def run_demo(self):
        """Run Demo executable"""
        Logger.info("Starting Demo...")
        
        if self.platform == "Windows":
            exe_name = "Demo.exe"
        else:
            exe_name = "Demo"
        
        # Find executable
        release_exe = self.build_dir / "Release" / exe_name
        debug_exe = self.build_dir / "Debug" / exe_name
        
        executable = None
        if release_exe.exists():
            executable = release_exe
        elif debug_exe.exists():
            executable = debug_exe
        else:
            executable = self._find_executable(exe_name)
        
        if not executable or not executable.exists():
            Logger.error("Executable not found, please run: python setup.py")
            sys.exit(1)
        
        print()
        Logger.separator()
        print(f"{Colors.GREEN}    Zoom Rooms Controller SDK Demo{Colors.NC}")
        Logger.separator()
        print()
        print("Commands: pair <code> | startmeeting | mute | endmeeting | unpair | quit")
        print()
        Logger.separator()
        print()
        
        # Run executable
        os.chdir(executable.parent)
        
        if self.platform == "Windows":
            # Copy DLL files from SDK dll directory to executable directory
            sdk_dll_dir = self.zrcsdk_root / "dll"
            if sdk_dll_dir.exists():
                dll_files = list(sdk_dll_dir.glob("*.dll"))
                if dll_files:
                    Logger.info(f"Copying {len(dll_files)} DLL file(s) to executable directory...")
                    dll_count = 0
                    for dll in dll_files:
                        try:
                            shutil.copy2(dll, executable.parent)
                            dll_count += 1
                        except Exception as e:
                            Logger.warning(f"Failed to copy {dll.name}: {e}")
                    if dll_count > 0:
                        Logger.success(f"Copied {dll_count} DLL file(s)")
                else:
                    Logger.warning(f"No DLL files found in {sdk_dll_dir}")
                
                # Also add to PATH as backup
                os.environ["PATH"] = f"{sdk_dll_dir};{os.environ.get('PATH', '')}"
            else:
                Logger.error(f"DLL directory not found: {sdk_dll_dir}")
                Logger.info("Please ensure the SDK dll directory exists at the same level as Demo/")
            
            # Run with subprocess to get better error handling
            try:
                result = subprocess.run([str(executable)], shell=False)
                if result.returncode != 0:
                    Logger.warning(f"Demo exited with code {result.returncode}")
                    
                    # Diagnose common Windows DLL errors
                    if result.returncode == 3221225781 or result.returncode == -1073741515:
                        # 0xC0000135 - DLL not found
                        Logger.error("Missing DLL files detected!")
                        print()
                        print("Troubleshooting steps:")
                        print(f"1. Check if DLL files exist in: {sdk_dll_dir}")
                        print(f"2. Verify DLLs copied to: {executable.parent}")
                        print("3. Install Visual C++ Redistributable:")
                        print("   https://aka.ms/vs/17/release/vc_redist.x64.exe")
                        print()
            except Exception as e:
                Logger.error(f"Failed to run Demo: {e}")
                Logger.info("This might be due to missing DLL files")
                Logger.info(f"Make sure required DLLs are in: {sdk_dll_dir}")
        else:
            os.system(f"./{executable.name}")
    
    def rebuild(self):
        """Rebuild Demo"""
        Logger.info("Rebuilding Demo...")
        
        # Ensure libuv is set up (for Linux)
        self.setup_libuv_linux()
        
        if not self.build_dir.exists():
            Logger.error("Build directory not found, please run: python setup.py")
            sys.exit(1)
        
        build_cmd = ["cmake", "--build", "build", "--config", "Release"]
        
        if self.run_command(build_cmd, cwd=self.demo_dir):
            Logger.success("Build successful!")
            print()
            try:
                response = input(f"{Colors.CYAN}Run Demo? [Y/n]{Colors.NC} ").strip().lower()
                if response != 'n':
                    self.run_demo()
            except KeyboardInterrupt:
                print()
        else:
            Logger.error("Build failed")
            sys.exit(1)
    
    def clean(self):
        """Clean build files"""
        Logger.warning("Cleaning build files...")
        print()
        print("Will clean:")
        print(f"  - Build directory: {self.build_dir}")
        
        env_file = self.zrcsdk_root / ("zrcsdk_env.bat" if self.platform == "Windows" else "zrcsdk_env.sh")
        if env_file.exists():
            print(f"  - Environment file: {env_file}")
        print()
        
        try:
            response = input(f"{Colors.YELLOW}Confirm clean? [y/N]{Colors.NC} ").strip().lower()
            if response == 'y':
                if self.build_dir.exists():
                    try:
                        shutil.rmtree(self.build_dir)
                        Logger.success("Removed build directory")
                    except PermissionError as e:
                        Logger.error(f"Cannot remove build directory: {e}")
                        print()
                        if self.platform == "Windows":
                            Logger.info("The directory is in use. Please:")
                            print("  1. Close Visual Studio")
                            print("  2. Close any running Demo.exe")
                            print("  3. Try again")
                        sys.exit(1)
                    except Exception as e:
                        Logger.error(f"Failed to remove build directory: {e}")
                        sys.exit(1)
                
                if env_file.exists():
                    env_file.unlink()
                    Logger.success("Removed environment file")
                Logger.success("Clean complete")
                print()
                Logger.info("To rebuild, run: python setup.py")
            else:
                Logger.info("Clean cancelled")
        except KeyboardInterrupt:
            print()
            Logger.info("Clean cancelled")
    
    def show_help(self):
        """Show help message"""
        print()
        print("Usage: python setup.py [command]")
        print()
        print("Commands:")
        print("  (no args)  - Setup and build SDK")
        print("  run        - Run Demo")
        print("  build      - Rebuild Demo")
        print("  clean      - Clean build files")
        print("  help       - Show this help")
        print()
        print("Examples:")
        if self.platform == "Windows":
            print("  python setup.py         # Initial setup")
            print("  python setup.py run     # Run Demo")
            print("  python setup.py build   # Rebuild")
            print("  python setup.py clean   # Clean")
        else:
            print("  python3 setup.py        # Initial setup")
            print("  python3 setup.py run    # Run Demo")
            print("  python3 setup.py build  # Rebuild")
            print("  python3 setup.py clean  # Clean")
        print()

def main():
    setup = ZRCSDKSetup()
    
    # Parse command
    command = sys.argv[1] if len(sys.argv) > 1 else ""
    
    if command == "run":
        setup.run_demo()
    elif command == "build":
        setup.rebuild()
    elif command == "clean":
        setup.clean()
    elif command in ["help", "-h", "--help"]:
        setup.show_help()
    elif command == "":
        # Full setup
        setup.show_welcome()
        setup.check_environment()
        setup.check_tools()
        setup.verify_sdk()
        setup.setup_libuv_linux()  # Setup libuv for Linux
        setup.setup_environment()
        setup.generate_and_build()
        setup.show_completion()
    else:
        Logger.error(f"Unknown command: {command}")
        setup.show_help()
        sys.exit(1)

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("\n\nOperation cancelled by user")
        sys.exit(1)
    except Exception as e:
        Logger.error(f"Unexpected error: {e}")
        sys.exit(1)


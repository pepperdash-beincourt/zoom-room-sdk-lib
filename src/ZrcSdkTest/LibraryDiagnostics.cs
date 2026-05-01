using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;
using Crestron.SimplSharp;

namespace ZrcSdkTest
{
    public static class LibraryDiagnostics
    {
        [DllImport("libdl.so.2")]
        private static extern IntPtr dlopen(string filename, int flags);

        [DllImport("libdl.so.2")]
        private static extern IntPtr dlerror();

        [DllImport("libdl.so.2")]
        private static extern int dlclose(IntPtr handle);
        
        [DllImport("libdl.so.2")]
        private static extern IntPtr dlsym(IntPtr handle, string symbol);

        private const int RTLD_NOW = 2;
        private const int RTLD_LAZY = 1;
        private const int RTLD_GLOBAL = 0x100;

        public static void RunDiagnostics()
        {
            var output = new StringBuilder();
            output.AppendLine("=== Crestron Library Diagnostics ===");
            output.AppendLine($"Date: {DateTime.Now}");
            output.AppendLine();

            // Get application directory
            var appDir = Crestron.SimplSharp.CrestronIO.Directory.GetApplicationDirectory();
            output.AppendLine($"Application Directory: {appDir}");
            output.AppendLine();

            // Check library files
            output.AppendLine("=== Library Files ===");
            CheckLibraryFile(appDir, "libZRCSdk.so", output);
            CheckLibraryFile(appDir, "libzrcsdkwrapperpdt.so", output);
            output.AppendLine();

            // System information
            output.AppendLine("=== System Information ===");
            try
            {
                output.AppendLine($"OS Version: {CrestronEnvironment.OSVersion.Version}");
                output.AppendLine($"Program Slot: {InitialParametersClass.ApplicationNumber}");
                output.AppendLine($"Room ID: {InitialParametersClass.RoomId}");
            }
            catch (Exception ex)
            {
                output.AppendLine($"Could not get system info: {ex.Message}");
            }
            output.AppendLine();

            // Try loading libraries with detailed error messages
            output.AppendLine("=== Library Load Tests ===");
            
            // Try loading ZRC SDK first with RTLD_GLOBAL
            output.AppendLine("Attempting to load ZRC SDK first (provides symbols for wrapper):");
            TestLibraryLoad(Path.Combine(appDir, "libZRCSdk.so"), output);
            output.AppendLine();
            
            output.AppendLine("Attempting to load Wrapper (depends on ZRC SDK):");
            TestLibraryLoad(Path.Combine(appDir, "libzrcsdkwrapperpdt.so"), output);
            output.AppendLine();

            // Check for common dependencies
            output.AppendLine("=== Dependency Tests ===");
            TestLibraryLoad("/lib/libstdc++.so.6", output);
            TestLibraryLoad("/usr/lib/libstdc++.so.6", output);
            TestLibraryLoad("/lib/libgcc_s.so.1", output);
            TestLibraryLoad("/usr/lib/libgcc_s.so.1", output);
            TestLibraryLoad("/lib/libpthread.so.0", output);
            TestLibraryLoad("/lib/libc.so.6", output);
            
            // Check for libatomic - often missing and required by modern C++ code
            output.AppendLine();
            output.AppendLine("Checking for libatomic (CRITICAL - required by ZRC SDK):");
            TestLibraryLoad("/lib/libatomic.so.1", output);
            TestLibraryLoad("/usr/lib/libatomic.so.1", output);
            TestLibraryLoad("/lib/libatomic.so", output);
            TestLibraryLoad("/usr/lib/libatomic.so", output);
            
            // Also check in application directory
            TestLibraryLoad(Path.Combine(appDir, "libatomic.so.1"), output);
            TestLibraryLoad(Path.Combine(appDir, "libatomic.so"), output);

            // Additional diagnostic - check if the ZRC SDK library might be for a different architecture
            output.AppendLine();
            output.AppendLine("=== Binary Format Check ===");
            output.AppendLine("Attempting to read library headers...");
            try
            {
                var sdkPath = Path.Combine(appDir, "libZRCSdk.so");
                if (File.Exists(sdkPath))
                {
          using var fs = File.OpenRead(sdkPath);
          var header = new byte[20];
          fs.Read(header, 0, 20);
          output.AppendLine($"libZRCSdk.so header: {BitConverter.ToString(header)}");

          // Check ELF magic
          if (header[0] == 0x7F && header[1] == 0x45 && header[2] == 0x4C && header[3] == 0x46)
          {
            output.AppendLine("  ✓ Valid ELF header");
            output.AppendLine($"  Class: {(header[4] == 1 ? "32-bit" : header[4] == 2 ? "64-bit" : "Unknown")}");
            output.AppendLine($"  Endian: {(header[5] == 1 ? "Little" : header[5] == 2 ? "Big" : "Unknown")}");
            output.AppendLine($"  Version: {header[6]}");
          }
          else
          {
            output.AppendLine("  ✗ Not a valid ELF file!");
          }
        }
                
                var wrapperPath = Path.Combine(appDir, "libzrcsdkwrapperpdt.so");
                if (File.Exists(wrapperPath))
                {
                    using (var fs = File.OpenRead(wrapperPath))
                    {
                        var header = new byte[20];
                        fs.Read(header, 0, 20);
                        output.AppendLine($"libzrcsdkwrapperpdt.so header: {BitConverter.ToString(header)}");;
                        
                        if (header[0] == 0x7F && header[1] == 0x45 && header[2] == 0x4C && header[3] == 0x46)
                        {
                            output.AppendLine("  ✓ Valid ELF header");
                            output.AppendLine($"  Class: {(header[4] == 1 ? "32-bit" : header[4] == 2 ? "64-bit" : "Unknown")}");
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                output.AppendLine($"Error reading headers: {ex.Message}");
            }

            // Shell-based diagnostics (4-series only)
            output.AppendLine();
            output.AppendLine("=== Shell-Based Diagnostics ===");
            RunShellDiagnostics(appDir, output);

            // Output results
            CrestronConsole.PrintLine(output.ToString());
            
            // Also try to write to a file
            try
            {
                var logPath = Path.Combine(appDir, "library_diagnostics.txt");
                File.WriteAllText(logPath, output.ToString().Replace("\n", "\r\n"));
                CrestronConsole.PrintLine($"\r\nDiagnostics saved to: {logPath}");
            }
            catch (Exception ex)
            {
                CrestronConsole.PrintLine($"Failed to write log file: {ex.Message}");
            }
        }

        private static void RunShellDiagnostics(string appDir, StringBuilder output)
        {
            output.AppendLine("Running shell commands for detailed library analysis...");
            output.AppendLine();

            try
            {
                // First check if commands exist
                output.AppendLine("--- Checking Tool Availability ---");
                bool hasFile = ShellExecutor.ExecuteCommandWithOutput("which file", out string filePath);
                output.AppendLine($"file command: {(hasFile && !string.IsNullOrWhiteSpace(filePath) ? filePath.Trim() : "NOT AVAILABLE")}");
                
                bool hasLdd = ShellExecutor.ExecuteCommandWithOutput("which ldd", out string lddPath);
                output.AppendLine($"ldd command: {(hasLdd && !string.IsNullOrWhiteSpace(lddPath) ? lddPath.Trim() : "NOT AVAILABLE")}");
                
                bool hasReadelf = ShellExecutor.ExecuteCommandWithOutput("which readelf", out string readelfPath);
                output.AppendLine($"readelf command: {(hasReadelf && !string.IsNullOrWhiteSpace(readelfPath) ? readelfPath.Trim() : "NOT AVAILABLE")}");
                
                output.AppendLine();

                // Check for libatomic specifically
                output.AppendLine("--- Critical Dependency Check ---");
                if (ShellExecutor.ExecuteCommandWithOutput("find /lib /usr/lib -name 'libatomic.so*' 2>/dev/null", out string atomicPaths))
                {
                    if (string.IsNullOrWhiteSpace(atomicPaths))
                    {
                        output.AppendLine("⚠️  WARNING: libatomic.so NOT FOUND in system - checking app directory...");
                        var appAtomicPath = Path.Combine(appDir, "libatomic.so.1");
                        if (File.Exists(appAtomicPath))
                        {
                            output.AppendLine($"✓ Found in app directory: {appAtomicPath}");
                        }
                        else
                        {
                            output.AppendLine("✗ NOT FOUND in app directory either!");
                        }
                        
                        // Do a more thorough search
                        output.AppendLine("Performing thorough system search...");
                        if (ShellExecutor.ExecuteCommandWithOutput("find / -name 'libatomic.so*' 2>/dev/null | head -n 20", out string allAtomicPaths))
                        {
                            if (!string.IsNullOrWhiteSpace(allAtomicPaths))
                            {
                                output.AppendLine("Found libatomic in these locations:");
                                output.AppendLine(allAtomicPaths);
                                
                                // Check each found libatomic
                                var atomicFiles = allAtomicPaths.Split('\n', StringSplitOptions.RemoveEmptyEntries);
                                foreach (var atomicFile in atomicFiles)
                                {
                                    var trimmedPath = atomicFile.Trim();
                                    if (!trimmedPath.Contains("/simpl/app02/") && File.Exists(trimmedPath))
                                    {
                                        output.AppendLine($"\nAnalyzing: {trimmedPath}");
                                        if (ShellExecutor.ExecuteCommandWithOutput($"readelf -A '{trimmedPath}' 2>&1 | grep -E '(Tag_CPU|Tag_ABI)' | head -n 5", out string foundAbi))
                                        {
                                            output.AppendLine($"  ABI: {foundAbi.Trim()}");
                                        }
                                        if (ShellExecutor.ExecuteCommandWithOutput($"readelf -V '{trimmedPath}' 2>&1 | grep GLIBC | head -n 3", out string foundGlibc))
                                        {
                                            output.AppendLine($"  GLIBC: {foundGlibc.Trim()}");
                                        }
                                        
                                        // Try loading it
                                        output.AppendLine($"  Testing load...");
                                        dlerror(); // Clear errors
                                        var testHandle = dlopen(trimmedPath, RTLD_LAZY);
                                        if (testHandle != IntPtr.Zero)
                                        {
                                            output.AppendLine($"  ✓ Successfully loaded!");
                                            dlclose(testHandle);
                                        }
                                        else
                                        {
                                            var err = dlerror();
                                            var errMsg = err != IntPtr.Zero ? Marshal.PtrToStringAnsi(err) : "Unknown";
                                            output.AppendLine($"  ✗ Failed: {errMsg}");
                                        }
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        output.AppendLine("libatomic.so locations:");
                        output.AppendLine(atomicPaths);
                    }
                }
                
                // Check GLIBC version and ABI compatibility
                output.AppendLine();
                output.AppendLine("--- System ABI/GLIBC Information ---");
                
                // Find the dynamic linker (critical for loading libraries)
                if (ShellExecutor.ExecuteCommandWithOutput("find /lib /usr/lib -name 'ld-*.so*' 2>/dev/null", out string linkerPaths))
                {
                    output.AppendLine("Dynamic linker locations:");
                    output.AppendLine(linkerPaths);
                }
                
                // Try multiple methods to get GLIBC version
                if (ShellExecutor.ExecuteCommandWithOutput("/lib/libc.so.6 2>&1 | head -n 1", out string glibcDirect))
                {
                    output.AppendLine($"GLIBC version (direct): {glibcDirect.Trim()}");
                }
                
                if (ShellExecutor.ExecuteCommandWithOutput("ldd --version 2>&1 | head -n 1", out string glibcVersion))
                {
                    output.AppendLine($"GLIBC version (ldd): {glibcVersion.Trim()}");
                }
                
                if (ShellExecutor.ExecuteCommandWithOutput("readelf -V /lib/libc.so.6 2>&1 | grep -A2 'Version definition'", out string glibcReadelf))
                {
                    if (!string.IsNullOrWhiteSpace(glibcReadelf))
                    {
                        output.AppendLine($"GLIBC versions available:");
                        output.AppendLine(glibcReadelf);
                    }
                }
                
                if (ShellExecutor.ExecuteCommandWithOutput("uname -m", out string arch))
                {
                    output.AppendLine($"Architecture: {arch.Trim()}");
                }
                
                if (ShellExecutor.ExecuteCommandWithOutput("uname -a", out string unameAll))
                {
                    output.AppendLine($"Kernel: {unameAll.Trim()}");
                }
                
                // Get OS/Distribution information
                if (ShellExecutor.ExecuteCommandWithOutput("cat /etc/os-release 2>/dev/null", out string osRelease))
                {
                    if (!string.IsNullOrWhiteSpace(osRelease))
                    {
                        output.AppendLine("OS Distribution (/etc/os-release):");
                        output.AppendLine(osRelease);
                    }
                }
                else if (ShellExecutor.ExecuteCommandWithOutput("cat /etc/*release 2>/dev/null | head -n 20", out string anyRelease))
                {
                    if (!string.IsNullOrWhiteSpace(anyRelease))
                    {
                        output.AppendLine("OS Distribution:");
                        output.AppendLine(anyRelease);
                    }
                }
                
                if (ShellExecutor.ExecuteCommandWithOutput("cat /proc/version 2>/dev/null", out string procVersion))
                {
                    if (!string.IsNullOrWhiteSpace(procVersion))
                    {
                        output.AppendLine($"Build info: {procVersion.Trim()}");
                    }
                }
                
                if (ShellExecutor.ExecuteCommandWithOutput("cat /proc/cpuinfo | grep 'model name' | head -n 1", out string cpu))
                {
                    output.AppendLine($"CPU: {cpu.Trim()}");
                }
                
                if (ShellExecutor.ExecuteCommandWithOutput("readelf -A /lib/libc.so.6 2>&1 | grep -E '(Tag_ABI|Tag_CPU)' | head -n 5", out string abiInfo))
                {
                    output.AppendLine("System ABI attributes:");
                    output.AppendLine(abiInfo);
                }
                
                // Check GLIBCXX and CXXABI versions available
                if (ShellExecutor.ExecuteCommandWithOutput("readelf -V /usr/lib/libstdc++.so.6 2>&1 | grep -E 'GLIBCXX_|CXXABI_' | head -n 30", out string cxxVersions))
                {
                    if (!string.IsNullOrWhiteSpace(cxxVersions))
                    {
                        output.AppendLine("C++ Library versions available (libstdc++):");
                        output.AppendLine(cxxVersions);
                    }
                }
                
                // Check our libatomic compatibility
                var localAtomicPath = Path.Combine(appDir, "libatomic.so.1");
                if (File.Exists(localAtomicPath))
                {
                    output.AppendLine();
                    output.AppendLine($"--- Checking {localAtomicPath} Compatibility ---");
                    
                    if (ShellExecutor.ExecuteCommandWithOutput($"readelf -d '{localAtomicPath}' 2>&1 | grep NEEDED", out string atomicNeeds))
                    {
                        output.AppendLine("Dependencies:");
                        output.AppendLine(atomicNeeds);
                    }
                    
                    if (ShellExecutor.ExecuteCommandWithOutput($"readelf -A '{localAtomicPath}' 2>&1 | grep -E '(Tag_ABI|Tag_CPU)' | head -n 5", out string atomicAbi))
                    {
                        output.AppendLine("ABI attributes:");
                        output.AppendLine(atomicAbi);
                    }
                    
                    if (ShellExecutor.ExecuteCommandWithOutput($"readelf -V '{localAtomicPath}' 2>&1 | grep -A5 'Version needs'", out string atomicGlibc))
                    {
                        output.AppendLine("GLIBC version requirements:");
                        output.AppendLine(atomicGlibc);
                    }
                }
                
                output.AppendLine();

                // Check libZRCSdk.so
                var sdkPath = Path.Combine(appDir, "libZRCSdk.so");
                if (File.Exists(sdkPath))
                {
                    output.AppendLine("--- libZRCSdk.so Analysis ---");
                    
                    // Check ELF interpreter (PT_INTERP) - this is critical!
                    if (ShellExecutor.ExecuteCommandWithOutput($"readelf -l '{sdkPath}' 2>&1 | grep -A2 'program interpreter'", out string interpInfo))
                    {
                        if (!string.IsNullOrWhiteSpace(interpInfo))
                        {
                            output.AppendLine("ELF Interpreter (PT_INTERP):");
                            output.AppendLine(interpInfo);
                        }
                    }
                    
                    // File type/architecture (if available)
                    if (hasFile && ShellExecutor.ExecuteCommandWithOutput($"file '{sdkPath}'", out string fileInfo))
                    {
                        output.AppendLine($"File Type: {fileInfo.Trim()}");
                    }
                    
                    // Check dependencies with ldd (if available)
                    if (hasLdd)
                    {
                        if (ShellExecutor.ExecuteCommandWithOutput($"ldd '{sdkPath}' 2>&1", out string lddOutput))
                        {
                            if (!string.IsNullOrWhiteSpace(lddOutput))
                            {
                                output.AppendLine("Dependencies (ldd):");
                                output.AppendLine(lddOutput);
                            }
                            else
                            {
                                output.AppendLine("Dependencies (ldd): No output (may indicate severe error)");
                            }
                        }
                    }
                    else
                    {
                        output.AppendLine("Dependencies: Cannot check (ldd not available)");
                    }
                    
                    // Check dynamic section with readelf
                    if (hasReadelf && ShellExecutor.ExecuteCommandWithOutput($"readelf -d '{sdkPath}' 2>&1", out string readelfOutput))
                    {
                        output.AppendLine("Dynamic Section (readelf -d):");
                        output.AppendLine(readelfOutput);
                    }
                    
                    output.AppendLine();
                }

                // Check libzrcsdkwrapperpdt.so
                var wrapperPath = Path.Combine(appDir, "libzrcsdkwrapperpdt.so");
                if (File.Exists(wrapperPath))
                {
                    output.AppendLine("--- libzrcsdkwrapperpdt.so Analysis ---");
                    
                    // File type/architecture
                    if (hasFile && ShellExecutor.ExecuteCommandWithOutput($"file '{wrapperPath}'", out string fileInfo))
                    {
                        output.AppendLine($"File Type: {fileInfo.Trim()}");
                    }
                    
                    // Check dependencies with ldd
                    if (hasLdd)
                    {
                        if (ShellExecutor.ExecuteCommandWithOutput($"ldd '{wrapperPath}' 2>&1", out string lddOutput))
                        {
                            if (!string.IsNullOrWhiteSpace(lddOutput))
                            {
                                output.AppendLine("Dependencies (ldd):");
                                output.AppendLine(lddOutput);
                            }
                            else
                            {
                                output.AppendLine("Dependencies (ldd): No output (may indicate severe error)");
                            }
                        }
                    }
                    
                    output.AppendLine();
                }

                // System library search paths
                output.AppendLine("--- System Library Information ---");
                
                if (ShellExecutor.ExecuteCommandWithOutput("ldconfig -p 2>&1 | grep -E 'stdc\\+\\+|gcc_s|atomic' | head -n 20", out string ldconfigOutput))
                {
                    if (!string.IsNullOrWhiteSpace(ldconfigOutput))
                    {
                        output.AppendLine("Available C++ Libraries:");
                        output.AppendLine(ldconfigOutput);
                    }
                    else
                    {
                        output.AppendLine("ldconfig: No output or not available");
                    }
                }
                
                // Check library search paths
                if (ShellExecutor.ExecuteCommandWithOutput("echo $LD_LIBRARY_PATH", out string ldPath))
                {
                    output.AppendLine($"LD_LIBRARY_PATH: {ldPath.Trim()}");
                }
                
                // Find libstdc++
                if (ShellExecutor.ExecuteCommandWithOutput("find /lib /usr/lib -name 'libstdc++.so*' 2>/dev/null | head -n 10", out string stdcppPaths))
                {
                    output.AppendLine("libstdc++ locations:");
                    output.AppendLine(stdcppPaths);
                }
                
                // Check for runtime linker errors
                output.AppendLine();
                output.AppendLine("--- Runtime Linker Test ---");
                if (ShellExecutor.ExecuteCommandWithOutput($"LD_DEBUG=libs LD_LIBRARY_PATH={appDir} /lib/ld-linux.so.3 '{sdkPath}' 2>&1 | head -n 50", out string ldDebug))
                {
                    output.AppendLine("Linker debug output:");
                    output.AppendLine(ldDebug);
                }
                
                output.AppendLine();
            }
            catch (Exception ex)
            {
                output.AppendLine($"Error running shell diagnostics: {ex.Message}");
                output.AppendLine("Note: Shell diagnostics require Crestron 4-series processor");
            }
        }

        private static void CheckLibraryFile(string dir, string filename, StringBuilder output)
        {
            var path = Path.Combine(dir, filename);
            output.AppendLine($"Checking: {filename}");
            
            if (File.Exists(path))
            {
                try
                {
                    var fileInfo = new FileInfo(path);
                    output.AppendLine($"  ✓ Found");
                    output.AppendLine($"  Size: {fileInfo.Length:N0} bytes");
                    output.AppendLine($"  Modified: {fileInfo.LastWriteTime}");
                }
                catch (Exception ex)
                {
                    output.AppendLine($"  ✗ Error reading file: {ex.Message}");
                }
            }
            else
            {
                output.AppendLine($"  ✗ NOT FOUND at {path}");
            }
        }

        private static void TestLibraryLoad(string path, StringBuilder output)
        {
            output.AppendLine($"Loading: {path}");
            
            if (!File.Exists(path))
            {
                output.AppendLine($"  ✗ File does not exist");
                return;
            }

            // Clear previous errors
            dlerror();

            // Try RTLD_LAZY | RTLD_GLOBAL (allows symbol resolution across libraries)
            var handle = dlopen(path, RTLD_LAZY | RTLD_GLOBAL);
            if (handle != IntPtr.Zero)
            {
                output.AppendLine($"  ✓ RTLD_LAZY|GLOBAL: Success (handle: 0x{handle.ToString("X")})");
                dlclose(handle);
                
                // Now try RTLD_NOW
                dlerror();
                handle = dlopen(path, RTLD_NOW | RTLD_GLOBAL);
                if (handle != IntPtr.Zero)
                {
                    output.AppendLine($"  ✓ RTLD_NOW|GLOBAL: Success");
                    dlclose(handle);
                }
                else
                {
                    var errorPtr = dlerror();
                    var errorMsg = errorPtr != IntPtr.Zero 
                        ? Marshal.PtrToStringAnsi(errorPtr) 
                        : "Unknown error";
                    output.AppendLine($"  ✗ RTLD_NOW|GLOBAL: {errorMsg}");
                }
            }
            else
            {
                var errorPtr = dlerror();
                var errorMsg = errorPtr != IntPtr.Zero 
                    ? Marshal.PtrToStringAnsi(errorPtr) 
                    : "Unknown error";
                output.AppendLine($"  ✗ RTLD_LAZY|GLOBAL: {errorMsg}");
                
                // Try without RTLD_GLOBAL to see if it makes a difference
                dlerror();
                handle = dlopen(path, RTLD_LAZY);
                if (handle != IntPtr.Zero)
                {
                    output.AppendLine($"  ✓ RTLD_LAZY (no GLOBAL): Success");
                    dlclose(handle);
                }
                else
                {
                    errorPtr = dlerror();
                    errorMsg = errorPtr != IntPtr.Zero 
                        ? Marshal.PtrToStringAnsi(errorPtr) 
                        : "Unknown error";
                    output.AppendLine($"  ✗ RTLD_LAZY (no GLOBAL): {errorMsg}");
                }
            }
        }
    }
}

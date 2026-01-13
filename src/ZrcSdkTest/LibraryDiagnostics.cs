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
            CheckLibraryFile(appDir, "libZrcSdkWrapper.so", output);
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
            TestLibraryLoad(Path.Combine(appDir, "libZrcSdkWrapper.so"), output);
            output.AppendLine();

            // Check for common dependencies
            output.AppendLine("=== Dependency Tests ===");
            TestLibraryLoad("/lib/libstdc++.so.6", output);
            TestLibraryLoad("/usr/lib/libstdc++.so.6", output);
            TestLibraryLoad("/lib/libgcc_s.so.1", output);
            TestLibraryLoad("/usr/lib/libgcc_s.so.1", output);
            TestLibraryLoad("/lib/libpthread.so.0", output);
            TestLibraryLoad("/lib/libc.so.6", output);

            // Additional diagnostic - check if the ZRC SDK library might be for a different architecture
            output.AppendLine();
            output.AppendLine("=== Binary Format Check ===");
            output.AppendLine("Attempting to read library headers...");
            try
            {
                var sdkPath = Path.Combine(appDir, "libZRCSdk.so");
                if (File.Exists(sdkPath))
                {
                    using (var fs = File.OpenRead(sdkPath))
                    {
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
                }
                
                var wrapperPath = Path.Combine(appDir, "libZrcSdkWrapper.so");
                if (File.Exists(wrapperPath))
                {
                    using (var fs = File.OpenRead(wrapperPath))
                    {
                        var header = new byte[20];
                        fs.Read(header, 0, 20);
                        output.AppendLine($"libZrcSdkWrapper.so header: {BitConverter.ToString(header)}");
                        
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

            // Output results
            CrestronConsole.PrintLine(output.ToString());
            
            // Also try to write to a file
            try
            {
                var logPath = Path.Combine(appDir, "library_diagnostics.txt");
                File.WriteAllText(logPath, output.ToString());
                CrestronConsole.PrintLine($"\nDiagnostics saved to: {logPath}");
            }
            catch (Exception ex)
            {
                CrestronConsole.PrintLine($"Failed to write log file: {ex.Message}");
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

using System;
using System.Runtime.InteropServices;
using System.Text;
using Crestron.SimplSharp;

namespace ZrcSdkTest
{
    /// <summary>
    /// Provides shell command execution capabilities on Crestron 4-series processors
    /// using P/Invoke to call Linux system functions
    /// </summary>
    public static partial class ShellExecutor
    {
        private const int BUFFER_SIZE = 4096;

        // Import popen - opens a pipe to a process created by forking and executing the shell command
        [LibraryImport("libc.so.6", StringMarshalling = StringMarshalling.Utf8)]
        private static partial IntPtr popen(string command, string mode);

        // Import pclose - closes a pipe opened by popen
        [LibraryImport("libc.so.6")]
        private static partial int pclose(IntPtr stream);

        // Import fgets - reads a line from a stream
        [LibraryImport("libc.so.6")]
        private static partial IntPtr fgets(byte[] buffer, int size, IntPtr stream);

        // Import system - executes a shell command
        [LibraryImport("libc.so.6", StringMarshalling = StringMarshalling.Utf8)]
        private static partial int system(string command);

        /// <summary>
        /// Executes a shell command and returns the exit code
        /// </summary>
        /// <param name="command">The shell command to execute</param>
        /// <returns>Exit code from the command</returns>
        public static int ExecuteCommand(string command)
        {
            try
            {
                CrestronConsole.PrintLine($"Executing: {command}");
                int exitCode = system(command);
                CrestronConsole.PrintLine($"Exit code: {exitCode}");
                return exitCode;
            }
            catch (Exception ex)
            {
                CrestronConsole.PrintLine($"Error executing command: {ex.Message}");
                ErrorLog.Error($"ShellExecutor.ExecuteCommand failed: {ex.Message}");
                return -1;
            }
        }

        /// <summary>
        /// Executes a shell command and captures its output
        /// </summary>
        /// <param name="command">The shell command to execute</param>
        /// <param name="output">The captured output from the command</param>
        /// <returns>True if successful, false otherwise</returns>
        public static bool ExecuteCommandWithOutput(string command, out string output)
        {
            output = string.Empty;
            IntPtr pipe = IntPtr.Zero;

            try
            {
                CrestronConsole.PrintLine($"Executing with output capture: {command}");
                
                // Open pipe to command
                pipe = popen(command, "r");
                if (pipe == IntPtr.Zero)
                {
                    CrestronConsole.PrintLine("Failed to open pipe");
                    return false;
                }

                // Read output
                var result = new StringBuilder();
                byte[] buffer = new byte[BUFFER_SIZE];
                
                while (true)
                {
                    IntPtr linePtr = fgets(buffer, BUFFER_SIZE, pipe);
                    if (linePtr == IntPtr.Zero)
                        break;

                    // Convert buffer to string
                    int length = 0;
                    while (length < buffer.Length && buffer[length] != 0)
                        length++;

                    if (length > 0)
                    {
                        string line = Encoding.UTF8.GetString(buffer, 0, length);
                        result.Append(line);
                    }

                    // Clear buffer for next read
                    Array.Clear(buffer, 0, buffer.Length);
                }

                output = result.ToString();
                CrestronConsole.PrintLine($"Captured {output.Length} bytes of output");
                return true;
            }
            catch (Exception ex)
            {
                CrestronConsole.PrintLine($"Error executing command: {ex.Message}");
                ErrorLog.Error($"ShellExecutor.ExecuteCommandWithOutput failed: {ex.Message}");
                return false;
            }
            finally
            {
                if (pipe != IntPtr.Zero)
                {
                    pclose(pipe);
                }
            }
        }

        /// <summary>
        /// Tests shell execution with common commands
        /// </summary>
        public static void RunTests()
        {
            CrestronConsole.PrintLine("=== Shell Executor Tests ===\r\n");

            // Test 1: Simple command with system()
            CrestronConsole.PrintLine("Test 1: Execute 'uname -a'");
            ExecuteCommand("uname -a");
            CrestronConsole.PrintLine("");

            // Test 2: Capture output
            CrestronConsole.PrintLine("Test 2: Capture output from 'ls -la /usr/bin | head -n 10'");
            if (ExecuteCommandWithOutput("ls -la /usr/bin | head -n 10", out string output))
            {
                CrestronConsole.PrintLine("Output:\r\n" + output);
            }
            CrestronConsole.PrintLine("");

            // Test 3: Check processor info
            CrestronConsole.PrintLine("Test 3: Get processor info");
            if (ExecuteCommandWithOutput("cat /proc/cpuinfo | grep 'model name' | head -n 1", out string cpuInfo))
            {
                CrestronConsole.PrintLine("CPU Info: " + cpuInfo);
            }
            CrestronConsole.PrintLine("");

            // Test 4: Check memory
            CrestronConsole.PrintLine("Test 4: Check memory");
            if (ExecuteCommandWithOutput("free -m", out string memInfo))
            {
                CrestronConsole.PrintLine("Memory Info:\r\n" + memInfo);
            }
            CrestronConsole.PrintLine("");

            // Test 5: List library dependencies
            CrestronConsole.PrintLine("Test 5: Check library dependencies in app directory");
            var appDir = Crestron.SimplSharp.CrestronIO.Directory.GetApplicationDirectory();
            var command = $"cd {appDir} && ls -la *.so 2>&1";
            if (ExecuteCommandWithOutput(command, out string libInfo))
            {
                CrestronConsole.PrintLine("Libraries:\r\n" + libInfo);
            }
            CrestronConsole.PrintLine("");
            CrestronConsole.PrintLine("\r\n=== Shell Executor Tests Complete ===\r\n");
        }
    }
}

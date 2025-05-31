# Change directory to the script location
Set-Location $PSScriptRoot

# Compile the program using Clang++
Write-Host " Compiling main.cpp..." -ForegroundColor Cyan
$compilationTime = Measure-Command {
    & "C:\Program Files\LLVM\bin\clang++.exe" -std=c++20 -fcolor-diagnostics -fansi-escape-codes -g -o "main.exe" `
        "main.cpp" 
}

# Check if compilation succeeded
if ($LASTEXITCODE -eq 0) {
    Write-Host " Compilation successful!" -ForegroundColor Green
    Write-Host (" Compilation time: {0:N2} seconds" -f $compilationTime.TotalSeconds) -ForegroundColor Yellow

    # Run the compiled executable
    Write-Host " Running main.exe..." -ForegroundColor Cyan
    & ".\main.exe"

    # Store exit code of main.exe
    $exitCode = $LASTEXITCODE

    # Print exit code of main.exe
    Write-Host " main.exe exited with code: $exitCode" -ForegroundColor Yellow
} else {
    Write-Host " Compilation failed! Exit code: $LASTEXITCODE" -ForegroundColor Red
}

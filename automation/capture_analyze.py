
import subprocess
import time
import os
import sys

def main():
    # Start simulator in background
    port = "9999"
    sim_proc = subprocess.Popen(["./build/simulator/device", port])
    time.sleep(1)  # let simulator start

    # Capture packets
    capture_file = "captured_packets.bin"
    print("Capturing packets...")
    subprocess.run(["./build/dissector/packet_analyzer", "capture", "127.0.0.1", port, capture_file], check=True)

    # Stop simulator
    sim_proc.terminate()
    sim_proc.wait()

    # Analyze captured file
    print("\nAnalyzing captured packets:")
    subprocess.run(["./build/dissector/packet_analyzer", "analyze", capture_file], check=True)

    # Cleanup
    os.remove(capture_file)

if __name__ == "__main__":
    main()

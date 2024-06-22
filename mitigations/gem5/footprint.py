def process_instruction_footprint(file_path, output_path):
    # Set to store unique instruction addresses
    unique_addresses = set()

    with open(file_path, 'r') as file:
        for line in file:
            # Split the line by commas and extract the physical address (5th column)
            parts = line.strip().split(',')
            if len(parts) >= 5:
                address = int(parts[4])
                unique_addresses.add(address)

    # Calculate the size of the footprint in bytes (assuming each instruction is 16 bytes)
    # footprint_size_bytes = len(unique_addresses) * 16 #X86
    footprint_size_bytes = len(unique_addresses) * 4  #ARM

    # Convert size to MB
    footprint_size_mb = footprint_size_bytes / (1024 * 1024)
    footprint_size_kb = footprint_size_bytes / 1024
    footprint_size_b  = footprint_size_bytes

    # Write unique addresses to the output file
    with open(output_path, 'w') as out_file:
        for address in sorted(unique_addresses):
            out_file.write(f"{address}\n")

    return unique_addresses, footprint_size_mb, footprint_size_kb, footprint_size_b


base_path = '/home/h255t794/SecSMT_Artifact/mitigations/gem5/results-trace/altra/iperf'
# File path to the input data
input_file_path = f'{base_path}/iperf-cpu1-trace-decode.txt'
# Output file path for unique addresses
output_file_path = f'{base_path}/iperf-cpu1-uni-addr.txt'


# Process the instruction footprint
unique_addresses, footprint_size_mb, footprint_size_kb, footprint_size_b = process_instruction_footprint(input_file_path, output_file_path)

# Print the results
print(f"Unique instruction addresses: {len(unique_addresses)}")
print(f"Instruction footprint size: {footprint_size_mb:.2f} MB")
print(f"Instruction footprint size: {footprint_size_kb:.2f} KB")
print(f"Instruction footprint size: {footprint_size_b:.2f} B")

import csv

def process_instruction_footprint(file_path, output_path, summary_path):
    # Dictionary to store unique instruction addresses and their access counts
    address_access_count = {}

    total_accesses = 0

    with open(file_path, 'r') as file:
        for line in file:
            # Split the line by commas and extract the physical address (5th column)
            parts = line.strip().split(',')
            if len(parts) >= 5:
                address = int(parts[4])
                if address in address_access_count:
                    address_access_count[address] += 1
                else:
                    address_access_count[address] = 1
                total_accesses += 1

    # Calculate the size of the footprint in bytes (assuming each instruction is 4 bytes for ARM)
    footprint_size_bytes = len(address_access_count) * 4  # ARM64 instruction size

    # Convert size to MB, KB, and B
    footprint_size_mb = footprint_size_bytes / (1024 * 1024)
    footprint_size_kb = footprint_size_bytes / 1024
    footprint_size_b  = footprint_size_bytes

    # Write unique addresses and their access counts to the output CSV file
    with open(output_path, 'w', newline='') as csv_file:
        csv_writer = csv.writer(csv_file)
        csv_writer.writerow(['Address', 'Access Count', 'Access Percentage'])
        for address in sorted(address_access_count.keys()):
            access_count = address_access_count[address]
            access_percentage = (access_count / total_accesses) * 100
            csv_writer.writerow([address, access_count, f"{access_percentage:.2f}"])

    # Write summary to the summary file
    with open(summary_path, 'w') as summary_file:
        summary_file.write(f"Unique instruction addresses: {len(address_access_count)}\n")
        summary_file.write(f"Instruction footprint size: {footprint_size_mb:.2f} MB\n")
        summary_file.write(f"Instruction footprint size: {footprint_size_kb:.2f} KB\n")
        summary_file.write(f"Instruction footprint size: {footprint_size_b:.2f} B\n")
        summary_file.write(f"Total instruction accesses: {total_accesses}\n")

    return address_access_count, footprint_size_mb, footprint_size_kb, footprint_size_b, total_accesses


base_path = '/home/h255t794/SecSMT_Artifact/mitigations/gem5/results-trace/altra/l2fwd'
# File path to the input data
input_file_path = f'{base_path}/l2fwd-fs-arm-trace-decode.txt'
# Output file path for unique addresses in CSV format
output_file_path = f'{base_path}/l2fwd-fs-arm-uni-addr-test.csv'
# Output file path for the summary print output
summary_file_path = f'{base_path}/out.txt'

# Process the instruction footprint
address_access_count, footprint_size_mb, footprint_size_kb, footprint_size_b, total_accesses = process_instruction_footprint(input_file_path, output_file_path, summary_file_path)

# Print the results (will also be saved in out.txt)
summary = (
    f"Unique instruction addresses: {len(address_access_count)}\n"
    f"Instruction footprint size: {footprint_size_mb:.2f} MB\n"
    f"Instruction footprint size: {footprint_size_kb:.2f} KB\n"
    f"Instruction footprint size: {footprint_size_b:.2f} B\n"
    f"Total instruction accesses: {total_accesses}\n"
)
print(summary)

# Save the summary to out.txt file
with open(summary_file_path, 'w') as summary_file:
    summary_file.write(summary)

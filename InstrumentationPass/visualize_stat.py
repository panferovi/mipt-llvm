import argparse
import matplotlib.pyplot as plt

parser = argparse.ArgumentParser()
parser.add_argument('build_dir')

args = parser.parse_args()

instruction_set = dict()
instruction_stat = dict()

with open(f"{args.build_dir}/instruction_stat.txt") as file:
    for line in file:
        stat = line.strip().split()
        name = stat[0]
        count = int(stat[2])
        opcode = int(stat[4])
        instruction_set[opcode] = name
        instruction_stat[name] = count

fig, ax = plt.subplots(2, figsize=(20, 40))

instruction_stat = dict(sorted(instruction_stat.items(), key=lambda item: item[1], reverse=True))
ax[0].xaxis.set_tick_params(rotation=90)
ax[0].bar(instruction_stat.keys(), instruction_stat.values())

instruction_trace = dict()

with open(f"{args.build_dir}/instructions.txt") as file:
    first_opcode = int(file.readline().strip())
    second_opcode = int(file.readline().strip())

    for third_line in file:
        first_name = instruction_set[first_opcode]
        second_name = instruction_set[second_opcode]
        pair_name = first_name + ":" + second_name
        if instruction_trace.get(pair_name) != None:
            instruction_trace[pair_name] += 1
        else:
            instruction_trace[pair_name] = 1

        first_opcode = second_opcode
        second_opcode = int(third_line.strip())

instruction_trace = dict(sorted(instruction_trace.items(), key=lambda item: item[1], reverse=True))
ax[1].xaxis.set_tick_params(rotation=90)
ax[1].bar(instruction_trace.keys(), instruction_trace.values())

plt.savefig('stat.png')
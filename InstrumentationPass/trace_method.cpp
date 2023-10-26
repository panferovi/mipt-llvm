#include <unordered_map>
#include <fstream>

struct InstrunctionInfo {
    uint64_t opcode {0};
    size_t counter {0};
};

static std::unordered_map<char *, InstrunctionInfo> instruction_set;
static std::fstream instructions_file("instructions.txt", instructions_file.out);


extern "C" void TraceInstruction(char *instruction, uint64_t opcode) {
    auto instr_iter = instruction_set.find(instruction);
    if (instr_iter == instruction_set.end()) {
        instruction_set.insert({instruction, {opcode, 1}});
    } else {
        instr_iter->second.counter++;
    }    
    instructions_file << opcode << "\n";
}

extern "C" void DumpInstructions() {
    std::fstream instructions_stat("instruction_stat.txt", instructions_stat.out);
    for (auto &&[instr, info]: instruction_set) {
        instructions_stat << instr << " counter: " << info.counter 
                          << " opcode: "  << info.opcode << "\n";
    }
}

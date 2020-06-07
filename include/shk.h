#pragma once

#include <stdint.h>

#include <iostream>
#include <optional>
#include <unordered_map>
#include <vector>

namespace shk {
	enum class opcode : uint8_t {
		noop    = 0b00000000,
		debug   = 0b00000001,
		halt    = 0b00000010,
		die     = 0b00000011,

		load    = 0b00000100,
		store   = 0b00000101,

		move    = 0b00001000,
		add     = 0b00001010,
		compare = 0b00001011,

		branch  = 0b00010000,
	};

	struct operand {
		enum class type : uint8_t {
			imm = 0b0,
			reg = 0b1,

			mem,
		};

		type ty;
		uint16_t value;
	};

	struct command {
		enum class type : uint8_t {
			eq = 0b0000,
		};

		type ty;
		std::vector<operand> operands;
	};

	struct instruction {
		opcode op;
		std::vector<operand> operands;
		std::vector<command> commands;
	};

	std::ostream & operator<<(std::ostream &os, opcode op) {
		os << "shk::opcode::";
		switch(op) {
		case opcode::noop:
			os << "noop";
			break;
		case opcode::debug:
			os << "debug";
			break;
		case opcode::halt:
			os << "halt";
			break;
		case opcode::die:
			os << "die";
			break;

		case opcode::load:
			os << "load";
			break;
		case opcode::store:
			os << "store";
			break;

		case opcode::move:
			os << "move";
			break;
		case opcode::add:
			os << "add";
			break;
		case opcode::compare:
			os << "compare";
			break;

		case opcode::branch:
			os << "branch";
			break;

		default:
			os << "<invalid (" << static_cast<int>(op) << ")>";
			break;
		}
		return os;
	}

	std::optional<opcode> mnemonic_to_opcode(const std::string &str) {
		const std::unordered_map<std::string, shk::opcode> mnemonics {
			{"NOP", shk::opcode::noop},
			{"DBG", shk::opcode::debug},
			{"HLT", shk::opcode::halt},
			{"DIE", shk::opcode::die},

			{"LOD", shk::opcode::load},
			{"STO", shk::opcode::store},

			{"MOV", shk::opcode::move},
			{"ADD", shk::opcode::add},
			{"CMP", shk::opcode::compare},

			{"BRA", shk::opcode::branch},
		};

		auto it = mnemonics.find(str);
		if(it == mnemonics.end()) {
			return {};
		}

		return it->second;
	}

	size_t num_operands(opcode op) {
		switch(op) {
		case opcode::noop:
		case opcode::debug:
		case opcode::halt:
		case opcode::die:
			return 0;
		case opcode::branch:
			return 1;
		case opcode::load:
		case opcode::store:
		case opcode::move:
			return 2;
		case opcode::add:
		case opcode::compare:
			return 3;
		default:
			return 0;
		}
	}
} // namespace shk

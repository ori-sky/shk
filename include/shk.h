#pragma once

#include <stdint.h>

#include <iostream>
#include <optional>
#include <unordered_map>
#include <vector>

namespace shk {
	enum class opcode : uint8_t {
		noop     = 0b00000000,
		debug    = 0b00000001,
		halt     = 0b00000010,
		die      = 0b00000011,

		load     = 0b00000100,
		store    = 0b00000101,

		move     = 0b00001000,
		add      = 0b00001010,
		compare  = 0b00001011,
		multiply = 0b00001100,

		branch   = 0b00010000,
	};

	struct operand {
		enum class type : uint8_t {
			imm = 0b0,
			reg = 0b1,
		};

		type ty;
		uint16_t value;
	};

	struct command {
		enum class type : uint8_t {
			eq = 0b0000,
			lt = 0b0001,
			le = 0b0010,
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
		case opcode::multiply:
			os << "multiply";
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

	std::ostream & operator<<(std::ostream &os, command::type ty) {
		os << "shk::command::type::";
		switch(ty) {
		case command::type::eq:
			os << "eq";
			break;
		case command::type::lt:
			os << "lt";
			break;
		case command::type::le:
			os << "le";
			break;
		default:
			os << "<invalid (" << static_cast<int>(ty) << ")>";
			break;
		}
		return os;
	}

	std::optional<opcode> mnemonic_to_opcode(const std::string &str) {
		const std::unordered_map<std::string, opcode> mnemonics {
			{"NOP", opcode::noop},
			{"DBG", opcode::debug},
			{"HLT", opcode::halt},
			{"DIE", opcode::die},

			{"LOD", opcode::load},
			{"STO", opcode::store},

			{"MOV", opcode::move},
			{"ADD", opcode::add},
			{"CMP", opcode::compare},
			{"MUL", opcode::multiply},

			{"BRA", opcode::branch},
		};

		auto it = mnemonics.find(str);
		if(it == mnemonics.end()) {
			return {};
		}

		return it->second;
	}

	std::optional<command::type> mnemonic_to_command(const std::string &str) {
		const std::unordered_map<std::string, command::type> mnemonics {
			{"EQ", command::type::eq},
			{"LT", command::type::lt},
			{"LE", command::type::le},
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
		case opcode::multiply:
			return 3;
		default:
			std::cerr << "error: num_operands: " << op << " not implemented" << std::endl;
			return 0;
		}
	}

	size_t num_operands(command::type ty) {
		switch(ty) {
		case command::type::eq:
		case command::type::lt:
		case command::type::le:
			return 1;
		default:
			std::cerr << "error: num_operands: " << ty << " not implemented" << std::endl;
			return 0;
		}
	}
} // namespace shk

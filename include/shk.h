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
		pop      = 0b00000110,
		push     = 0b00000111,

		move     = 0b00001000,
		add      = 0b00001010,
		compare  = 0b00001011,
		multiply = 0b00001100,

		branch   = 0b00010000,
		call     = 0b00010001,
		ret      = 0b00010010,

		get_ip   = 0b00010100,
		set_ip   = 0b00010101,
		get_sp   = 0b00010110,
		set_sp   = 0b00010111,

		data,
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
		case opcode::pop:
			os << "pop";
			break;
		case opcode::push:
			os << "push";
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
		case opcode::call:
			os << "call";
			break;
		case opcode::ret:
			os << "ret";
			break;

		case opcode::get_ip:
			os << "get_ip";
			break;
		case opcode::set_ip:
			os << "set_ip";
			break;
		case opcode::get_sp:
			os << "get_sp";
			break;
		case opcode::set_sp:
			os << "set_sp";
			break;

		case opcode::data:
			os << "<data>";
			break;

		default:
			os << "<invalid (" << static_cast<int>(op) << ")>";
			break;
		}
		return os;
	}

	size_t num_operands(opcode op) {
		switch(op) {
		case opcode::noop:
		case opcode::debug:
		case opcode::halt:
		case opcode::die:
		case opcode::ret:
			return 0;
		case opcode::pop:
		case opcode::push:
		case opcode::branch:
		case opcode::call:
		case opcode::get_ip:
		case opcode::set_ip:
		case opcode::get_sp:
		case opcode::set_sp:
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

	struct operand {
		enum class type : uint8_t {
			imm   = 0b00,
			reg   = 0b01,
			deref = 0b10,
			label,
		};

		type ty;
		uint16_t value;
		std::string label;
	};

	std::ostream & operator<<(std::ostream &os, const operand::type ty) {
		os << "shk::operand::type::";
		switch(ty) {
		case operand::type::imm:
			os << "imm";
			break;
		case operand::type::reg:
			os << "reg";
			break;
		case operand::type::deref:
			os << "deref";
			break;
		case operand::type::label:
			os << "<label>";
			break;
		default:
			os << "<invalid (" << static_cast<int>(ty) << ")>";
			break;
		}
		return os;
	}

	struct command {
		enum class type : uint8_t {
			eq = 0b0000,
			lt = 0b0001,
			le = 0b0010,
			gt = 0b0011,
			ge = 0b0100,
		};

		type ty;
		std::vector<operand> operands;
	};

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
		case command::type::gt:
			os << "gt";
			break;
		case command::type::ge:
			os << "ge";
			break;
		default:
			os << "<invalid (" << static_cast<int>(ty) << ")>";
			break;
		}
		return os;
	}

	size_t num_operands(command::type ty) {
		switch(ty) {
		case command::type::eq:
		case command::type::lt:
		case command::type::le:
		case command::type::gt:
		case command::type::ge:
			return 1;
		default:
			std::cerr << "error: num_operands: " << ty << " not implemented" << std::endl;
			return 0;
		}
	}

	struct instruction {
		opcode op;
		std::vector<operand> operands;
		std::vector<command> commands;

		size_t size() const {
			size_t n = 0;

			if(op != opcode::data) {
				++n;
			}
			n += operands.size();
			for(auto &cmd : commands) {
				++n;
				n += cmd.operands.size();
			}

			return n;
		}
	};

	std::optional<opcode> mnemonic_to_opcode(std::string_view str) {
		const std::unordered_map<std::string, opcode> mnemonics {
			{"NOP", opcode::noop},
			{"DBG", opcode::debug},
			{"HLT", opcode::halt},
			{"DIE", opcode::die},

			{"LOD", opcode::load},
			{"STO", opcode::store},
			{"POP", opcode::pop},
			{"PSH", opcode::push},

			{"MOV", opcode::move},
			{"ADD", opcode::add},
			{"CMP", opcode::compare},
			{"MUL", opcode::multiply},

			{"BRA", opcode::branch},
			{"CAL", opcode::call},
			{"RET", opcode::ret},

			{"GIP", opcode::get_ip},
			{"SIP", opcode::set_ip},
			{"GSP", opcode::get_sp},
			{"SSP", opcode::set_sp},

			{"DAT", opcode::data},
		};

		auto it = mnemonics.find(std::string(str));
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
			{"GT", command::type::gt},
			{"GE", command::type::ge},
		};

		auto it = mnemonics.find(str);
		if(it == mnemonics.end()) {
			return {};
		}

		return it->second;
	}
} // namespace shk

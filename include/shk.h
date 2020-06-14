#pragma once

#include <stdint.h>

#include <iostream>
#include <memory>
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
		divide   = 0b00001101,
		modulo   = 0b00001110,

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
		case opcode::divide:
			os << "divide";
			break;
		case opcode::modulo:
			os << "modulo";
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
		case opcode::divide:
		case opcode::modulo:
			return 3;
		default:
			throw std::runtime_error("invalid opcode");
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
		std::unique_ptr<operand> segment;
		std::string label;

		size_t size() const {
			size_t n = 0;

			++n;
			if(segment) {
				++n;
			}

			return n;
		}

		char type_char() const {
			switch(ty) {
			case type::imm:
				return '#';
			case type::reg:
				return '$';
			case type::deref:
				return '*';
			default:
				throw std::runtime_error("invalid type char");
			}
		}
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

	std::ostream & operator<<(std::ostream &os, const operand &oper) {
		return os << "shk::operand{ty=" << oper.ty << ", value=" << oper.value << '}';
	}

	struct command {
		enum class type : uint8_t {
			eq = 0b0000,
			ne = 0b0001,
			lt = 0b0010,
			le = 0b0011,
			gt = 0b0100,
			ge = 0b0101,
		};

		type ty;
		std::vector<operand> operands;

		size_t size() const {
			size_t n = 0;

			++n;
			for(auto &oper : operands) {
				n += oper.size();
			}

			return n;
		}
	};

	std::ostream & operator<<(std::ostream &os, command::type ty) {
		os << "shk::command::type::";
		switch(ty) {
		case command::type::eq:
			os << "eq";
			break;
		case command::type::ne:
			os << "ne";
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
		case command::type::ne:
		case command::type::lt:
		case command::type::le:
		case command::type::gt:
		case command::type::ge:
			return 1;
		default:
			throw std::runtime_error("invalid command type");
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
			for(auto &oper : operands) {
				n += oper.size();
			}
			for(auto &cmd : commands) {
				n += cmd.size();
			}

			return n;
		}
	};

	std::ostream & operator<<(std::ostream &os, const instruction &instr) {
		os << "shk::instruction {" << std::endl;
		os << "  op = " << instr.op << std::endl;
		for(size_t i = 0; i < instr.operands.size(); ++i) {
			os << "  operands[" << i << "] = " << instr.operands[i] << std::endl;
		}
		//for(size_t i = 0; i < instr.commands; ++i) {
		//	os << "  commands[" << i << "] = " << instr.commands[i] << std::endl;
		//}
		return os << '}';
	}

	opcode mnemonic_to_opcode(const std::string_view sv) {
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
			{"DIV", opcode::divide},
			{"MOD", opcode::modulo},

			{"BRA", opcode::branch},
			{"CAL", opcode::call},
			{"RET", opcode::ret},

			{"GIP", opcode::get_ip},
			{"SIP", opcode::set_ip},
			{"GSP", opcode::get_sp},
			{"SSP", opcode::set_sp},

			{"DAT", opcode::data},
		};

		std::string str(sv);

		auto it = mnemonics.find(str);
		if(it == mnemonics.end()) {
			throw std::runtime_error("invalid mnemonic: " + str);
		}

		return it->second;
	}

	std::string opcode_to_mnemonic(const opcode op) {
		const std::unordered_map<opcode, std::string> opcodes {
			{opcode::noop,     "NOP"},
			{opcode::debug,    "DBG"},
			{opcode::halt,     "HLT"},
			{opcode::die,      "DIE"},

			{opcode::load,     "LOD"},
			{opcode::store,    "STO"},
			{opcode::pop,      "POP"},
			{opcode::push,     "PSH"},

			{opcode::move,     "MOV"},
			{opcode::add,      "ADD"},
			{opcode::compare,  "CMP"},
			{opcode::multiply, "MUL"},
			{opcode::divide,   "DIV"},
			{opcode::modulo,   "MOD"},

			{opcode::branch,   "BRA"},
			{opcode::call,     "CAL"},
			{opcode::ret,      "RET"},

			{opcode::get_ip,   "GIP"},
			{opcode::set_ip,   "SIP"},
			{opcode::get_sp,   "GSP"},
			{opcode::set_sp,   "SSP"},

			{opcode::data,     "DAT"},
		};

		auto it = opcodes.find(op);
		if(it == opcodes.end()) {
			throw std::runtime_error("invalid opcode");
		}

		return it->second;
	}

	command::type mnemonic_to_command(const std::string_view sv) {
		const std::unordered_map<std::string, command::type> mnemonics {
			{"EQ", command::type::eq},
			{"NE", command::type::ne},
			{"LT", command::type::lt},
			{"LE", command::type::le},
			{"GT", command::type::gt},
			{"GE", command::type::ge},
		};

		std::string str(sv);

		auto it = mnemonics.find(str);
		if(it == mnemonics.end()) {
			throw std::runtime_error("invalid mnemonic: " + str);
		}

		return it->second;
	}

	std::string command_to_mnemonic(const command::type ty) {
		const std::unordered_map<command::type, std::string> types {
			{command::type::eq, "EQ"},
			{command::type::ne, "NE"},
			{command::type::lt, "LT"},
			{command::type::le, "LE"},
			{command::type::gt, "GT"},
			{command::type::ge, "GE"},
		};

		auto it = types.find(ty);
		if(it == types.end()) {
			throw std::runtime_error("invalid command type");
		}

		return it->second;
	}
} // namespace shk

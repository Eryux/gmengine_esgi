#pragma once

#ifndef SERIALIZABLETYPEINFO_H
#define SERIALIZABLETYPEINFO_H

#include <typeinfo>
#include <fstream>

template<typename T>
class SerializableTypeInfo : std::type_info {

public:
	SerializableTypeInfo() {

	}

	~SerializableTypeInfo() {

	}

	void serialize(std::ostream stream) {
		stream << name();
	}

};

#endif
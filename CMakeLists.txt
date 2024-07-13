cmake_minimum_required(VERSION 3.27)

project(compiler)
add_executable(compiler main.cpp)
target_sources(
	compiler PRIVATE
	ast.cpp
	ast.h
	parser.cpp
	parser.h
)

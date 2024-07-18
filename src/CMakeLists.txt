target_sources(
	compiler PRIVATE
	compiler.cpp
	compiler.h
	ast_builder.cpp
	ast_builder.h
	IO.cpp
	IO.h
	parser.cpp
	parser.h
	types.h
	utils.h
	utils.cpp
)

target_include_directories(compiler PUBLIC
	"${PROJECT_SOURCE_DIR}/src"
)

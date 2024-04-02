import os
import re
from datetime import datetime

def traverse_directory(path):
    relevant_files = []

    # Define the subdirectories to search
    search_dirs = ["source", "cmake"]

    # Add files from the project root directory
    for file in os.listdir(path):
        if file.endswith((".cpp", ".h", "CMakeLists.txt", ".cmake")):
            relevant_files.append(os.path.join(path, file))

    # Search the specified subdirectories
    for subdir in search_dirs:
        subdir_path = os.path.join(path, subdir)
        if os.path.exists(subdir_path):
            for root, dirs, files in os.walk(subdir_path):
                for file in files:
                    if file.endswith((".cpp", ".h", "CMakeLists.txt", ".cmake")):
                        relevant_files.append(os.path.join(root, file))

    return relevant_files

def extract_cpp_h_info(filepath):
    info = []

    with open(filepath, 'r', encoding='utf-8', errors='replace') as file:
        content = file.read()

    # File path
    info.append(f"File: {filepath}")

    # Namespace declarations
    namespaces = re.findall(r'namespace\s+([\w\d_]+)', content)
    for ns in namespaces:
        info.append(f"Namespace: {ns}")

    # Class declarations with inheritance information
    class_declarations = re.findall(r'class\s+([\w\d_]+)(\s*:\s*(public|private|protected)\s*([\w\d_]+))?', content)
    for class_decl in class_declarations:
        if class_decl[1]:
            info.append(f"Class: {class_decl[0]} inherits {class_decl[2]} {class_decl[3]}")
        else:
            info.append(f"Class: {class_decl[0]}")

    # Function declarations
    function_declarations = re.findall(r'((?:[\w\d_]+::)?[\w\d_]+)\s*(?:\s+|\s*::\s*)([\w\d_]+)\s*\(([^)]*)\)\s*(const)?\s*(?={|;)', content)
    for func_decl in function_declarations:
        return_type, func_name, params, const = func_decl
        const = 'const' if const else ''
        info.append(f"Function: {return_type} {func_name}({params}) {const}".strip())

    # Struct declarations
    struct_declarations = re.findall(r'struct\s+([\w\d_]+)', content)
    for struct_decl in struct_declarations:
        info.append(f"Struct: {struct_decl}")

    # Enum declarations
    enum_declarations = re.findall(r'enum\s+(class)?\s*([\w\d_]+)', content)
    for enum_decl in enum_declarations:
        enum_class = 'class' if enum_decl[0] else ''
        info.append(f"Enum {enum_class}: {enum_decl[1]}")

    # Preprocessor directives (#define and #include statements)
    directives = re.findall(r'(#(?:define|include)\s+.+)', content)
    for directive in directives:
        info.append(directive.strip())

    return info

def extract_cmake_info(filepath):
    info = []

    with open(filepath, 'r', encoding='utf-8', errors='replace') as file:
        content = file.read()

    # File path
    info.append(f"File: {filepath}")

    # Project name
    project_name = re.search(r'project\s*\(\s*([\w\d_-]+)', content, re.IGNORECASE)
    if project_name:
        info.append(f"Project: {project_name.group(1)}")

    # Executable targets
    executable_targets = re.findall(r'add_executable\s*\(\s*([\w\d_-]+)', content, re.IGNORECASE)
    for exe_target in executable_targets:
        info.append(f"Executable Target: {exe_target}")

    # Library targets
    library_targets = re.findall(r'add_library\s*\(\s*([\w\d_-]+)', content, re.IGNORECASE)
    for lib_target in library_targets:
        info.append(f"Library Target: {lib_target}")

    # Include directories
    include_directories = re.findall(r'include_directories\s*\(\s*([\w\d_/\.-]+)', content, re.IGNORECASE)
    for include_dir in include_directories:
        info.append(f"Include Directory: {include_dir}")

    # Dependencies
    find_package_calls = re.findall(r'find_package\s*\(\s*([\w\d_-]+)', content, re.IGNORECASE)
    for package in find_package_calls:
        info.append(f"Dependency (find_package): {package}")

    target_link_libraries_calls = re.findall(r'target_link_libraries\s*\(\s*([\w\d_-]+)\s+([\w\d_/\.-]+)', content, re.IGNORECASE)
    for target, lib in target_link_libraries_calls:
        info.append(f"Dependency (target_link_libraries): {target} -> {lib}")

    return info

def create_summary_file(summary_data, output_path):
    with open(output_path, 'w', encoding='utf-8') as summary_file:
        for line in summary_data:
            summary_file.write(line + '\n')

def main():
    # Get the project root directory assuming the script is located in the project root
    project_root = os.path.dirname(os.path.realpath(__file__))

    # Get the current date and format it as a string
    current_date = datetime.now().strftime('%Y-%m-%d')

    # Append the date string to the output filename
    output_file = f"{project_root}/project_summary_{current_date}.txt"

    print("Collecting file data...")
    file_data = traverse_directory(project_root)
    summary_data = []

    print("Processing files...")
    for filepath in file_data:
        print(f"Processing {filepath}")
        if filepath.endswith((".cpp", ".h")):
            summary_data.extend(extract_cpp_h_info(filepath))
        elif filepath.endswith(("CMakeLists.txt", ".cmake")):
            summary_data.extend(extract_cmake_info(filepath))

    print("Creating summary file...")
    create_summary_file(summary_data, output_file)

    print(f"Summary file created: {output_file}")

if __name__ == "__main__":
    main()
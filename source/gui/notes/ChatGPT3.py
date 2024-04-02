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
    with open(filepath, 'r', encoding='utf-8') as file:
        content = file.read()

    class_declarations = re.findall(r'(class|struct)\s+(\w+)(\s+:\s*(public|private|protected)\s+(\w+))?', content)
    member_variables = re.findall(r'[\w:]+\s+(\w+)\s*(\*|\&)?\s*(\w+);', content)
    function_declarations = re.findall(r'([\w:]+)\s+(\w+)\s*\(([\w\s,<>\[\].:&\*]*)\)', content)
    enum_declarations = re.findall(r'(enum\s+(class)?\s*(\w+))', content)

    info = [f"File: {filepath}"]

    if class_declarations:
        info.append("Inheritance Hierarchy:")
        for class_decl in class_declarations:
            inheritance = f" inherits {class_decl[3]} {class_decl[4]}" if class_decl[2] else ''
            info.append(f"  {'Class' if class_decl[0] == 'class' else 'Struct'}: {class_decl[1]}{inheritance}")

    for member_var in member_variables:
        info.append(f"Member Variable: {member_var[0]}{' '.join(member_var[1:])}")

    for func_decl in function_declarations:
        info.append(f"Function: {func_decl[0]} {func_decl[1]}({func_decl[2]})")

    for enum_decl in enum_declarations:
        enum_class = f"Enum class" if enum_decl[1] else "Enum"
        info.append(f"{enum_class}: {enum_decl[2]}")

    return info


def main():
    project_path = os.path.dirname(os.path.realpath(__file__))
    relevant_files = traverse_directory(project_path)

    project_info = []
    for filepath in relevant_files:
        project_info.extend(extract_cpp_h_info(filepath))

    now = datetime.now()
    timestamp = now.strftime("%Y%m%d_%H%M%S")
    output_file = f"{project_path}/project_summary_{timestamp}.txt"

    with open(output_file, 'w', encoding='utf-8') as file:
        for line in project_info:
            file.write(line + '\n')

    print(f"Project summary saved as: {output_file}")


if __name__ == "__main__":
    main()
    
    
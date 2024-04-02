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
    class_declarations = re.findall(
        r'class\s+([\w\d_]+)(\s*:\s*(public|private|protected)\s*([\w\d_]+))?', content)
    inheritance_hierarchy = {}
    if class_declarations:
        #info.append("Inheritance Hierarchy:")
        for class_decl in class_declarations:
            if class_decl[1]:
                inheritance_hierarchy[class_decl[0]] = f"{class_decl[2]} {class_decl[3]}"
            else:
                inheritance_hierarchy[class_decl[0]] = None
        for class_name, inheritance in inheritance_hierarchy.items():
            if inheritance:
                info.append(f"  Class: {class_name} inherits {inheritance}")
            else:
                info.append(f"  Class: {class_name}")

    # Member variables
    member_variables = re.findall(
        r'(?:public|private|protected):\s+([\w\d_<>,:\s\*&]+)\s+([\w\d_]+)\s*(?:\[[^]]*\])?\s*;', content)
    for member_var in member_variables:
        info.append(f"Member Variable: {member_var[0]} {member_var[1]}")

    # Function declarations
    function_declarations = re.findall(
        r'((?:[\w\d_]+::)?[\w\d_]+)\s+([\w\d_]+)\s*\(([^)]*)\)\s*(const)?\s*(?={|;)(?! else)', content)
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
        enum_class = f"Enum class" if enum_decl[1] else "Enum"
        info.append(f"{enum_class}: {enum_decl[1]}")

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
    
    
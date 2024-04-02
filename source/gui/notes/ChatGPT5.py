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

def remove_comments(content):
    comment_pattern = r'//[^\n]*\n'
    content = re.sub(comment_pattern, '\n', content)
    return content

def remove_empty_lines(content):
    empty_line_pattern = r'\n\s*\n'
    content = re.sub(empty_line_pattern, '\n', content)
    return content
    
def collapse_function_bodies(content):
    # Regular expression to collapse function bodies
    func_body_pattern = r'\)((\s*const)|(\s*noexcept))*\s*{((?:[^{}]|({\s...\s}))*)}'

    # Replace function bodies with ellipses
    content = re.sub(func_body_pattern, ') { ... }', content)

    return content
    
def collapse_tries(content):
    try_body_pattern = r'(try|catch)\s*{((?:[^{}]|({\s...\s}))*)}'
    
    # Replace function bodies with ellipses
    content = re.sub(try_body_pattern, 'try { ... }', content)

    return content
    
def collapse_init_list(content):    
    # Regular expression to collapse initialization lists
    init_list_pattern = r'\)\s*\:[^{}]*{\s...\s}'

    # Replace function bodies with ellipses
    content = re.sub(init_list_pattern, ') { ... }', content)

    return content
    
def extract_cpp_h_info(filepath):
    info = []

    with open(filepath, 'r', encoding='utf-8', errors='replace') as file:
        content = file.read()

    # File path
    info.append(f"\n*****\nFile: {filepath}")
    
    content = remove_empty_lines(content)
    
    for i in range(5):
        content = collapse_function_bodies(content)
        content = collapse_tries(content)
        content = remove_comments(content)
        content = remove_empty_lines(content)
        content = collapse_init_list(content)
    
    info.append(content)
    
    return info
    
def extract_cmake_info(filepath):
    info = []

    with open(filepath, 'r', encoding='utf-8', errors='replace') as file:
        content = file.read()

    # File path
    info.append(f"\n*****\nFile: {filepath}")

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

def main():
    # Get the project root directory assuming the script is located in the project root
    project_path = os.path.dirname(os.path.realpath(__file__))
    relevant_files = traverse_directory(project_path)

    summary_data = []
    print("Processing files...")
    for filepath in relevant_files:
        print(f"Processing {filepath}")
        if filepath.endswith((".cpp", ".h")):
            summary_data.extend(extract_cpp_h_info(filepath))
        elif filepath.endswith(("CMakeLists.txt", ".cmake")):
            summary_data.extend(extract_cmake_info(filepath))

    # Get the current date and format it as a string
    now = datetime.now()
    timestamp = now.strftime("%Y%m%d_%H%M%S")
    
    # Append the date string to the output filename
    output_file = f"{project_path}/project_summary_{timestamp}.txt"

    with open(output_file, 'w', encoding='utf-8') as file:
        for line in summary_data:
            file.write(line + '\n')

    print(f"Project summary saved as: {output_file}")


if __name__ == "__main__":
    main()
    
    
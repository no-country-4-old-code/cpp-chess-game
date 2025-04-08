import re
import utils.utils as utils
from collections import defaultdict


def parse_includes(file_path):
    """Extract included files from a source file."""
    include_pattern = re.compile(r'#include\s+["<](.*?)[">]')
    includes = set()
    with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
        for line in f:
            match = include_pattern.search(line)
            if match:
                includes.add(match.group(1))
    return includes


def build_dependency_graph(files):
    """Construct a dependency graph from the source files."""
    graph = defaultdict(set)
    reverse_graph = defaultdict(set)
    
    for filename, path in files.items():
        includes = parse_includes(path)
        for include in includes:
            # only consider local files
            if include in files:  
                graph[filename].add(include)
                reverse_graph[include].add(filename)
    return graph, reverse_graph


def analyze_dependencies(files):
    graph, reverse_graph = build_dependency_graph(files)
    
    print("\n=== Dependency Analysis:")
    for file, dependencies in graph.items():
        print(f"{file:<20} depends on {dependencies}")
    
    print("\n=== Dependency Counts:")
    print(f"{'Filename':<12} | {'Incoming':^12} | {'Outgoing':^12}")
    print("-" * 40)
    for file in files:
        incoming = len(reverse_graph[file])
        outgoing = len(graph[file])
        print(f"{file:<12} | (->) {incoming:^7} | (<-) {outgoing:^7}")
    print("\n")
    

if __name__ == "__main__":
    directory = utils.get_input_arguments()
    files = utils.find_source_files(directory)
    analyze_dependencies(files)

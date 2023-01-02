import sys
import os

SFML_FLAGS = '-lsfml-graphics -lsfml-window -lsfml-system'
MY_FLAGS = '-Wall -Wextra -std=c++20 -Ofast'

if len(sys.argv) == 2 and sys.argv[1] == 'clean':
	if os.path.exists('build'):
		from pathlib import Path

		def rmdir(directory):
			directory = Path(directory)

			for item in directory.iterdir():
				if item.is_dir():
					rmdir(item)
				else:
					item.unlink()
			directory.rmdir()

		rmdir('build')
	print('Cleaned build directory')
else:
	import subprocess
	from glob import iglob
	import mmap
	import json
	from collections import defaultdict
	
	cpp_paths_set = set([f.replace('\\', '/') for f in iglob('src/**/*',     recursive=True) if os.path.isfile(f) and f.endswith('.cpp')])
	hpp_paths_set = set([f.replace('\\', '/') for f in iglob('include/**/*', recursive=True) if os.path.isfile(f) and f.endswith('.hpp')])
	# cpp_paths_set = set(sorted(cpp_paths_set))
	# hpp_paths_set = set(sorted(hpp_paths_set))

	cpp_dependencies: dict[str, list[str]] = defaultdict(list)
	hpp_dependent: dict[str, list[str]] = defaultdict(list)

	for cpp_path in cpp_paths_set:
		file = open(cpp_path, 'rb', 0)
		fancy_map = mmap.mmap(file.fileno(), 0, access=mmap.ACCESS_READ)
		
		for hpp_path in hpp_paths_set:
			hpp_file_name = os.path.basename(hpp_path)
			hpp_file_name_buffer = hpp_file_name.encode('utf-8')
			
			if fancy_map.find(hpp_file_name_buffer) != -1:
				cpp_dependencies[cpp_path].append(hpp_path)
				hpp_dependent[hpp_path].append(cpp_path)
		
		# cpp_dependencies[cpp_path] = list(sorted(cpp_dependencies[cpp_path]))
	
	cpp_path_to_build_list: set[str] = set({})
	old_status: dict[str, float] = {}
	status: dict[str, float] = {}
	build_executable = False
	
	if os.path.exists('build'):
		if os.path.exists('build/status.json'):
			with open('build/status.json', 'r') as status_file:
				old_status = json.load(status_file)

			for cpp_path in cpp_paths_set:
				if cpp_path in old_status.keys() and old_status[cpp_path] == (cpp_path_mtime := os.path.getmtime(cpp_path)):
					status[cpp_path] = cpp_path_mtime
					continue
				
				print(cpp_path, "was added/modified")
				
				build_executable = True
				cpp_path_to_build_list.add(cpp_path)
					
			for hpp_path in hpp_paths_set:
				if hpp_path in old_status.keys() and old_status[hpp_path] == (hpp_path_mtime := os.path.getmtime(hpp_path)):
					status[hpp_path] = hpp_path_mtime
					continue
				
				print(hpp_path, "was added/modified")
				build_executable = True
				
				for cpp_path in hpp_dependent[hpp_path]:
					cpp_path_to_build_list.add(cpp_path)
					
		else:
			cpp_path_to_build_list = cpp_paths_set
			build_executable = True
	else:
		os.mkdir('build')
		
		cpp_path_to_build_list = cpp_paths_set
		build_executable = True

	for cpp_path in cpp_path_to_build_list:
		built_obj_path = 'build'
		
		dirs = cpp_path.split('/')[:-1]
		
		for dir in dirs:
			built_obj_path += '/' + dir

			if os.path.exists(built_obj_path):
				break
			else:
				os.mkdir(built_obj_path)
		
		include_list = list(set([os.path.dirname(dep) for dep in cpp_dependencies[cpp_path]]))
		INCLUDE_FLAGS = ' '.join(['-I ' + flag for flag in include_list])

		built_obj_path = 'build/' + cpp_path + '.o'
		
		command = ' '.join(['g++ -c', MY_FLAGS])
		command = ' '.join([command, '-o', built_obj_path, cpp_path])
		command = ' '.join([command, SFML_FLAGS, INCLUDE_FLAGS])
		
		print(command)

		try:
			subprocess.check_output(command, shell=True)

			status[cpp_path] = os.path.getmtime(cpp_path)

			for hpp_file_path in cpp_dependencies[cpp_path]:
				status[hpp_file_path] = os.path.getmtime(hpp_file_path)
		except:
			build_executable = False
	
	if build_executable or not os.path.exists('runme.exe'):
		full_obj_list = ['build/' + f + '.o' for f in cpp_paths_set]

		command = ' '.join(['g++ -o runme.exe', MY_FLAGS])
		command = ' '.join([command, ' '.join(full_obj_list)])
		command = ' '.join([command, SFML_FLAGS])
		
		print(command)

		try:
			output = subprocess.check_output(command, shell=True)
		except:
			pass
	
	with open('build/status.json', 'w') as status_file:
		json.dump(status, status_file, indent=4)
	
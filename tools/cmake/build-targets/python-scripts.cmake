include(create-python-script-target)

set(SCRIPT_FOLDER_PATH ${TOOLS_FOLDER_PATH}/scripts)

create_target_python_script(${SCRIPT_FOLDER_PATH}/find-todos.py)
create_target_python_script(${SCRIPT_FOLDER_PATH}/detect-dependencies.py)

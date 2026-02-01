#!/usr/bin/env python3
"""
Скрипт для компиляции GLSL шейдеров из иерархической директории spirv в compiled.
Сохраняет структуру директорий.
Компилирует все .vert и .frag файлы используя glslc.exe.
"""

import os
import subprocess
import sys
from pathlib import Path

# Определяем пути
SCRIPT_DIR = Path(__file__).parent
SPIRV_DIR = SCRIPT_DIR / "spirv"
COMPILED_DIR = SCRIPT_DIR / "compiled"
GLSL_C = SCRIPT_DIR / "glslc.exe"  # glslc.exe в корне shaders/


def compile_shader(input_file: Path, output_file: Path) -> bool:
    """
    Компилирует один шейдер из input_file в output_file.
    
    Args:
        input_file: Путь к исходному GLSL файлу
        output_file: Путь к выходному SPIR-V файлу
        
    Returns:
        True если компиляция успешна, False иначе
    """
    if not GLSL_C.exists():
        print(f"Ошибка: не найден компилятор {GLSL_C}")
        return False
    
    # Создаем директорию для выходных файлов, если её нет
    output_file.parent.mkdir(parents=True, exist_ok=True)
    
    try:
        # Запускаем glslc для компиляции
        cmd = [
            str(GLSL_C),
            str(input_file),
            "-o",
            str(output_file)
        ]
        
        # Определяем рабочую директорию - директория исходного файла
        working_dir = input_file.parent
        
        result = subprocess.run(
            cmd,
            capture_output=True,
            text=True,
            cwd=working_dir
        )
        
        if result.returncode == 0:
            # Вычисляем относительный путь для красивого вывода
            rel_input = input_file.relative_to(SPIRV_DIR)
            rel_output = output_file.relative_to(COMPILED_DIR)
            print(f"✓ Скомпилирован: {rel_input} -> {rel_output}")
            return True
        else:
            rel_input = input_file.relative_to(SPIRV_DIR)
            print(f"✗ Ошибка компиляции {rel_input}:")
            if result.stderr:
                print(result.stderr)
            return False
            
    except Exception as e:
        rel_input = input_file.relative_to(SPIRV_DIR)
        print(f"✗ Исключение при компиляции {rel_input}: {e}")
        return False


def find_shader_files(root_dir: Path):
    """
    Рекурсивно находит все файлы шейдеров (.vert и .frag).
    
    Args:
        root_dir: Корневая директория для поиска
        
    Returns:
        Список путей к файлам шейдеров
    """
    shader_extensions = {'.vert', '.frag'}
    shader_files = []
    
    # Рекурсивно обходим директории
    for current_dir, dirs, files in os.walk(root_dir):
        current_path = Path(current_dir)
        
        for file in files:
            file_path = current_path / file
            if file_path.suffix in shader_extensions:
                shader_files.append(file_path)
    
    return shader_files


def get_output_path(input_file: Path) -> Path:
    """
    Определяет путь для выходного файла на основе входного.
    
    Args:
        input_file: Путь к исходному файлу шейдера
        
    Returns:
        Путь для скомпилированного файла
    """
    # Вычисляем относительный путь относительно SPIRV_DIR
    rel_path = input_file.relative_to(SPIRV_DIR)
    
    # Меняем расширение на .spv
    output_name = rel_path.with_suffix(rel_path.suffix + '.spv')
    
    # Формируем полный путь в COMPILED_DIR
    return COMPILED_DIR / output_name


def main():
    """Основная функция скрипта."""
    # Проверяем существование директорий
    if not SPIRV_DIR.exists():
        print(f"Ошибка: директория {SPIRV_DIR} не найдена")
        sys.exit(1)
    
    # Проверяем наличие компилятора
    if not GLSL_C.exists():
        print(f"Ошибка: не найден компилятор {GLSL_C}")
        print(f"Ожидается по пути: {GLSL_C}")
        sys.exit(1)
    
    # Находим все файлы шейдеров рекурсивно
    shader_files = find_shader_files(SPIRV_DIR)
    
    if not shader_files:
        print(f"Не найдено файлов шейдеров в {SPIRV_DIR} и поддиректориях")
        sys.exit(0)
    
    print(f"Найдено файлов для компиляции: {len(shader_files)}")
    print("Структура директорий будет сохранена.")
    print("-" * 50)
    
    # Компилируем каждый файл
    success_count = 0
    for shader_file in shader_files:
        output_file = get_output_path(shader_file)
        
        if compile_shader(shader_file, output_file):
            success_count += 1
    
    print("-" * 50)
    print(f"Компиляция завершена: {success_count}/{len(shader_files)} успешно")
    
    # Выводим структуру скомпилированных файлов
    if success_count > 0:
        print("\nСтруктура скомпилированных файлов:")
        for path in sorted(COMPILED_DIR.rglob("*.spv")):
            rel_path = path.relative_to(COMPILED_DIR)
            print(f"  {rel_path}")
    
    if success_count < len(shader_files):
        sys.exit(1)


if __name__ == "__main__":
    main()
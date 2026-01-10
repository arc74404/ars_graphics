#!/usr/bin/env python3
"""
Скрипт для компиляции GLSL шейдеров из директории spirv в compiled.
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
GLSL_C = SPIRV_DIR / "glslc.exe"


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
        
        result = subprocess.run(
            cmd,
            capture_output=True,
            text=True,
            cwd=SPIRV_DIR
        )
        
        if result.returncode == 0:
            print(f"✓ Скомпилирован: {input_file.name} -> {output_file.name}")
            return True
        else:
            print(f"✗ Ошибка компиляции {input_file.name}:")
            if result.stderr:
                print(result.stderr)
            return False
            
    except Exception as e:
        print(f"✗ Исключение при компиляции {input_file.name}: {e}")
        return False


def main():
    """Основная функция скрипта."""
    # Проверяем существование директорий
    if not SPIRV_DIR.exists():
        print(f"Ошибка: директория {SPIRV_DIR} не найдена")
        sys.exit(1)
    
    # Находим все файлы шейдеров (.vert и .frag)
    shader_extensions = {'.vert', '.frag'}
    shader_files = [
        f for f in SPIRV_DIR.iterdir()
        if f.is_file() and f.suffix in shader_extensions
    ]
    
    if not shader_files:
        print(f"Не найдено файлов шейдеров в {SPIRV_DIR}")
        sys.exit(0)
    
    print(f"Найдено файлов для компиляции: {len(shader_files)}")
    print("-" * 50)
    
    # Компилируем каждый файл
    success_count = 0
    for shader_file in shader_files:
        # Формируем имя выходного файла: имя исходного + .spv
        output_name = shader_file.name + ".spv"
        output_file = COMPILED_DIR / output_name
        
        if compile_shader(shader_file, output_file):
            success_count += 1
    
    print("-" * 50)
    print(f"Компиляция завершена: {success_count}/{len(shader_files)} успешно")
    
    if success_count < len(shader_files):
        sys.exit(1)


if __name__ == "__main__":
    main()

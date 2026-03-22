# Инструкция по развертыванию и использованию модели Qwen в Google Colab

## 1. Обновление системы
Перед началом работы обновите список пакетов:
```bash
sudo apt update
```

## 2. Установка tmate (SSH-доступ)
Для удаленного подключения к сессии установите tmate:
```bash
sudo apt install tmate -y
```
После установки запустите `tmate` – вы получите SSH-ссылку для доступа к терминалу.

> **Примечание:** Используйте `!tmate` для мгновенного получения ссылки.

## 3. Установка Miniconda
Скачайте и установите Miniconda для управления окружениями Python:
```bash
wget https://repo.anaconda.com/miniconda/Miniconda3-latest-Linux-x86_64.sh
bash Miniconda3-latest-Linux-x86_64.sh
source ~/.bashrc
```
Проверьте установку:
```bash
conda --version
```

## 4. Принятие условий использования Conda
Для работы с каналами Conda необходимо принять лицензионные соглашения:
```bash
conda tos accept --override-channels --channel https://repo.anaconda.com/pkgs/main
conda tos accept --override-channels --channel https://repo.anaconda.com/pkgs/r
```

## 5. Создание окружения для модели Qwen
Создайте новое окружение с Python 3.10 и активируйте его:
```bash
conda create -n qwen python=3.10 -y
conda activate qwen
```

## 6. Установка библиотек для работы с LLM
Внутри окружения установите необходимые пакеты:
```bash
pip install transformers accelerate bitsandbytes
```

## 7. Тестовый запуск модели (однократный)
Создайте файл `run_qwen.py` с содержимым:
```python
from transformers import AutoModelForCausalLM, AutoTokenizer
import torch

model_name = "Qwen/Qwen3.5-4B-Base"

tokenizer = AutoTokenizer.from_pretrained(model_name)

model = AutoModelForCausalLM.from_pretrained(
    model_name,
    device_map="auto",
    torch_dtype=torch.float16
)

prompt = "Write a Python function that calculates factorial"

inputs = tokenizer(prompt, return_tensors="pt").to(model.device)

output = model.generate(
    **inputs,
    max_new_tokens=200
)

print(tokenizer.decode(output[0]))
```
Запустите скрипт:
```bash
python run_qwen.py
```

## 8. Интерактивный чат с моделью
Для общения в реальном времени создайте `run_qwen_chat.py`:
```python
from transformers import AutoModelForCausalLM, AutoTokenizer
import torch

model_name = "Qwen/Qwen3.5-4B-Base"

tokenizer = AutoTokenizer.from_pretrained(model_name)
model = AutoModelForCausalLM.from_pretrained(
    model_name,
    device_map="auto",
    torch_dtype=torch.float16
)

while True:
    prompt = input(">>> ")

    inputs = tokenizer(prompt, return_tensors="pt").to(model.device)

    output = model.generate(
        **inputs,
        max_new_tokens=200
    )

    print(tokenizer.decode(output[0]))
```
Запустите:
```bash
python run_qwen_chat.py
```

## 9. Автоматизация браузера с Playwright
Установите Playwright и необходимые браузеры:
```bash
pip install playwright
playwright install
```

Создайте файл `run_qwen_wiki.py` для демонстрации управления браузером через модель:
```python
from transformers import AutoModelForCausalLM, AutoTokenizer
import torch
from playwright.sync_api import sync_playwright
import json

# Загрузка модели
model_name = "Qwen/Qwen3.5-4B-Base"
tokenizer = AutoTokenizer.from_pretrained(model_name)
model = AutoModelForCausalLM.from_pretrained(
    model_name,
    device_map="auto",
    torch_dtype=torch.float16
)

# Запуск Playwright
playwright = sync_playwright().start()
browser = playwright.chromium.launch(headless=False)
page = browser.new_page()

# Открываем начальную страницу
page.goto("https://en.wikipedia.org")

while True:
    prompt = input(">>> ")

    # Генерируем JSON-команду
    inputs = tokenizer(prompt, return_tensors="pt").to(model.device)
    output = model.generate(**inputs, max_new_tokens=200)
    response = tokenizer.decode(output[0])

    print("MODEL:", response)

    # Попытка выполнить команду из JSON
    try:
        cmd = json.loads(response)
        action = cmd.get("action")
        if action == "click":
            page.click(cmd["selector"])
        elif action == "fill":
            page.fill(cmd["selector"], cmd["value"])
        elif action == "goto":
            page.goto(cmd["url"])
        else:
            print("Unknown action")
    except Exception as e:
        print("Cannot parse JSON or execute command:", e)
```
Запустите пример:
```bash
python run_qwen_wiki.py
```
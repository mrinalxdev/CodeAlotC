import datetime
import json
import os
import subprocess
import threading
import time
import queue
import ttkbootstrap as ttk
from ttkbootstrap.constants import *
from tkinter import messagebox

CONFIG_FILE = 'reminder_config.json'
REMINDER_INTERVAL = 60
task_queue = queue.Queue()

class Reminder:
    def __init__(self, name, reminder_time, command=None, recurring=False):
        self.name = name
        self.reminder_time = reminder_time
        self.command = command
        self.recurring = recurring

    def to_dict(self):
        return {
            'name': self.name,
            'reminder_time': self.reminder_time.strftime('%Y-%m-%d %H:%M:%S'),
            'command': self.command,
            'recurring': self.recurring
        }

    @classmethod
    def from_dict(cls, reminder_dict):
        reminder_time = datetime.datetime.strptime(reminder_dict['reminder_time'], '%Y-%m-%d %H:%M:%S')
        return cls(reminder_dict['name'], reminder_time, reminder_dict.get('command'), reminder_dict.get('recurring', False))

def load_reminders():
    try:
        with open(CONFIG_FILE, 'r') as f:
            reminders = json.load(f)
            return [Reminder.from_dict(reminder) for reminder in reminders]
    except FileNotFoundError:
        return []

def save_reminders(reminders):
    reminders_dict = [reminder.to_dict() for reminder in reminders]
    with open(CONFIG_FILE, 'w') as f:
        json.dump(reminders_dict, f)

def add_reminder(name, reminder_time, command=None, recurring=False):
    reminders = load_reminders()
    reminders.append(Reminder(name, reminder_time, command, recurring))
    save_reminders(reminders)

def check_reminders():
    reminders = load_reminders()
    current_time = datetime.datetime.now()
    updated_reminders = []
    for reminder in reminders:
        if reminder.reminder_time <= current_time:
            task_queue.put(reminder)  # Put the reminder in the task queue
            if reminder.command:
                subprocess.run(reminder.command, shell=True)
            if reminder.recurring:
                reminder.reminder_time += datetime.timedelta(days=1)
                updated_reminders.append(reminder)
        else:
            updated_reminders.append(reminder)
    save_reminders(updated_reminders)

def threaded_checker():
    while True:
        check_reminders()
        time.sleep(REMINDER_INTERVAL)

def show_reminder_popup(reminder):
    def mark_done():
        reminder_window.destroy()

    def snooze():
        reminder.reminder_time += datetime.timedelta(minutes=10)
        save_reminders(load_reminders())
        reminder_window.destroy()

    reminder_window = ttk.Toplevel()
    reminder_window.title("Reminder")

    label = ttk.Label(reminder_window, text=f"Task: {reminder.name}\nDue: {reminder.reminder_time}")
    label.pack(pady=10)

    done_button = ttk.Button(reminder_window, text="Mark as Done", bootstyle="success-outline", command=mark_done)
    done_button.pack(side=LEFT, padx=10, pady=10)

    snooze_button = ttk.Button(reminder_window, text="Snooze 10 min", bootstyle="warning-outline", command=snooze)
    snooze_button.pack(side=RIGHT, padx=10, pady=10)

    reminder_window.mainloop()

def process_tasks():
    while not task_queue.empty():
        reminder = task_queue.get()
        show_reminder_popup(reminder)

def main_gui():
    app = ttk.Window(themename="darkly")  # Dark theme
    app.title("Reminder Bot")
    app.geometry("400x300")

    def add_reminder_gui():
        def save_new_reminder():
            name = name_entry.get()
            reminder_time = datetime.datetime.strptime(time_entry.get(), "%Y-%m-%d %H:%M:%S")
            command = command_entry.get()
            recurring = recurring_var.get()
            add_reminder(name, reminder_time, command, recurring == 1)
            add_reminder_window.destroy()

        add_reminder_window = ttk.Toplevel(app)
        add_reminder_window.title("Add Reminder")

        ttk.Label(add_reminder_window, text="Reminder Name").grid(row=0, column=0, padx=5, pady=5)
        name_entry = ttk.Entry(add_reminder_window)
        name_entry.grid(row=0, column=1, padx=5, pady=5)

        ttk.Label(add_reminder_window, text="Reminder Time (YYYY-MM-DD HH:MM:SS)").grid(row=1, column=0, padx=5, pady=5)
        time_entry = ttk.Entry(add_reminder_window)
        time_entry.grid(row=1, column=1, padx=5, pady=5)

        ttk.Label(add_reminder_window, text="Command (Optional)").grid(row=2, column=0, padx=5, pady=5)
        command_entry = ttk.Entry(add_reminder_window)
        command_entry.grid(row=2, column=1, padx=5, pady=5)

        recurring_var = ttk.IntVar()
        ttk.Checkbutton(add_reminder_window, text="Recurring", variable=recurring_var, bootstyle="success-round-toggle").grid(row=3, columnspan=2, pady=5)

        save_button = ttk.Button(add_reminder_window, text="Save", bootstyle="success", command=save_new_reminder)
        save_button.grid(row=4, columnspan=2, pady=10)

    def list_reminders_gui():
        reminders = load_reminders()
        reminder_list_window = ttk.Toplevel(app)
        reminder_list_window.title("List of Reminders")

        for idx, reminder in enumerate(reminders):
            reminder_label = ttk.Label(reminder_list_window, text=f"{idx+1}. {reminder.name}: {reminder.reminder_time}")
            reminder_label.pack(pady=2)

    reminder_button = ttk.Button(app, text="Add Reminder", bootstyle="primary-outline", command=add_reminder_gui)
    reminder_button.pack(pady=10)

    list_reminder_button = ttk.Button(app, text="List Reminders", bootstyle="info-outline", command=list_reminders_gui)
    list_reminder_button.pack(pady=10)

    def periodic_task_processing():
        process_tasks()  # Process the tasks (reminders)
        app.after(1000, periodic_task_processing)  # Call this function every second

    periodic_task_processing()
    app.mainloop()

if __name__ == "__main__":
    threading.Thread(target=threaded_checker, daemon=True).start()# Background thread for reminders
    main_gui() 

# # import tkinter as tk

# # class ToggleButtonGUI(tk.Frame):
# #     def __init__(self, master=None):
# #         super().__init__(master)
# #         self.master = master
# #         self.pack()
# #         self.create_widgets()

# #     def create_widgets(self):
# #         self.toggle_var = tk.IntVar(value=1)
# #         #self.toggle_var.set(1)
# #         self.toggle_button = tk.Checkbutton(self, text="Toggle", variable=self.toggle_var,
# #                                             onvalue=2, offvalue=1, 
# #                                             activebackground = 'red',
# #                                             # activeforeground = 'red'
# #                                             # selectcolor = 'red'
# #                                             highlightcolor = 'red'
# #                                             )
# #         self.toggle_button.pack()

# #         self.print_button = tk.Button(self, text="Print status", command=self.print_status)
# #         self.print_button.pack()

# #     def print_status(self):
# #         print("Toggle status:", self.toggle_var.get())

# # root = tk.Tk()
# # app = ToggleButtonGUI(master=root)
# # app.mainloop()



# import tkinter as tk

# class ToggleButtonGUI(tk.Frame):
#     def __init__(self, master=None):
#         super().__init__(master)
#         self.master = master
#         self.pack()
#         self.create_widgets()

#     def create_widgets(self):
#         self.toggle_var = tk.IntVar(value=2)  # Set default value to ON
#         self.toggle_button = tk.Checkbutton(self, text="#", variable=self.toggle_var,
#                                             onvalue=2, offvalue=1, 
#                                             background='blue', 
#                                             activebackground='light blue',
#                                             # ontext="#", 
#                                             # offtext=""
#                                             )
#         self.toggle_button.pack()

#         self.print_button = tk.Button(self, text="Print status", command=self.print_status)
#         self.print_button.pack()

#     def print_status(self):
#         print("Toggle status:", self.toggle_var.get())

# root = tk.Tk()
# app = ToggleButtonGUI(master=root)
# app.mainloop()


# # import tkinter as tk

# # class CustomToggleButton(tk.Canvas):
# #     def __init__(self, parent, width, height, bg_color, fg_color, font, active_bg_color, active_fg_color, active_text, inactive_text, default_value):
# #         super().__init__(parent, width=width, height=height, bg=bg_color, highlightthickness=0)
# #         self.value = default_value
# #         self.active_bg_color = active_bg_color
# #         self.active_fg_color = active_fg_color
# #         self.active_text = active_text
# #         self.inactive_text = inactive_text

# #         self.create_oval(1, 1, height-1, height-1, fill=fg_color, outline="")
# #         self.create_text(height + 10, height // 2, text=self.inactive_text, fill=fg_color, anchor=tk.W, font=font)

# #         self.bind("<Button-1>", self.toggle)

# #     def toggle(self, event):
# #         if self.value == 1:
# #             self.itemconfig(1, fill=self.active_bg_color)
# #             self.itemconfig(2, text=self.active_text, fill=self.active_fg_color)
# #             self.value = 2
# #         else:
# #             self.itemconfig(1, fill=self["bg"])
# #             self.itemconfig(2, text=self.inactive_text, fill=self["bg"])
# #             self.value = 1

# #     def get_value(self):
# #         return self.value

# # class ToggleButtonGUI(tk.Frame):
# #     def __init__(self, master=None):
# #         super().__init__(master)
# #         self.master = master
# #         self.pack()
# #         self.create_widgets()

# #     def create_widgets(self):
# #         self.toggle_button = CustomToggleButton(self, 50, 30, "gray", "white", "Arial 12 bold", "pink", "white", "#", "", 2)
# #         self.toggle_button.pack()

# #         self.print_button = tk.Button(self, text="Print status", command=self.print_status)
# #         self.print_button.pack()

# #     def print_status(self):
# #         print("Toggle status:", self.toggle_button.get_value())

# # root = tk.Tk()
# # app = ToggleButtonGUI(master=root)
# # app.mainloop()


with open( '/home/inttdev/map/RC-2S_map-v2.txt' ) as file :
     for line in file :
         print(len(line),' and ',line[ int(len(line)) - 2 ])


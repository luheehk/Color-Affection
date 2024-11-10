# Color-Affection

The **Color-Affection** program is written in C and allows users to transform an audio file by inputting RGB color values. These RGB values adjust three effectors—**Delay**, **Decay**, and **Threshold**—to create unique audio transformations based on the selected color.

## Instructions

Follow these steps to use the Color-Affection program:

### 1. Download and Setup
- Download the **Color_Affection** folder to your system.
- Place the audio file you wish to modify in the **Color_Affection** folder.

### 2. Running the Program
- Open the **ColorPalette_run.c** file in a text editor.
- Launch a **Terminal** session.

### 3. Specify the Audio File
- In **ColorPalette_run.c**, locate `kInputFileName` and enter the exact file name and extension of your audio file.

### 4. Compile the Program
- Navigate to the **Color_Affection** directory in Terminal.
- Compile the program with the following command:
  ```bash
  clang ColorAffection_run.c -lsndfile

### 5. Input RGB Values
- Follow Terminal's instructions to sequentially enter **R**, **G**, and **B** values.
  - **Tip:** To determine HSV values based on your RGB inputs, you can use online tools like an RGB to HSV converter.
  - **Alternative:** Experiment with values in advance using a Color Wheel to approximate the color effect.
  - **Note:** Ensure RGB values are between `0` and `255`.

### 6. View Conversion Details
- Terminal will display the converted **HSV** values based on your RGB inputs.
- Observe how the effectors **Delay**, **Decay**, and **Threshold** are adjusted in Terminal output.

### 7. Access the Output File
- The transformed audio file is saved as **HSV.wav** in the **Color_Affection** folder.
- To customize the output filename, modify the `kOutputFileName` variable in the **ColorPalette_run.c** file.

### 8. Final Check
- Your modified audio file will be saved automatically in the same folder as **ColorAffection_run.c**.

You’re now ready to create your own color-influenced audio files using Color-Affection!


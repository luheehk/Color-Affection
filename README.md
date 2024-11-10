# Color-Affection

The Color-Affection program is a C-based application that allows users to transform an audio file by inputting RGB color values. These RGB values adjust three effectors—**Delay**, **Decay**, and **Threshold**—to create unique audio transformations based on the selected color. Now, **color your sound**, shaping its character and texture through *Color-Affection*!

## Instructions

Follow these steps to use the Color-Affection program:

### 1. Download and Setup
- Download the **Color_Affection** folder to your system.
- Place the audio file you wish to modify in the **Color_Affection** folder with the filename **allenFM.wav**.

### 2. Running the Program
- Open **main.c** (or the file containing the `main` function) in a text editor if needed.
- Launch a **Terminal** session.

### 3. Specify the Audio File
- Ensure your input audio file is named `allenFM.wav` in the **Color_Affection** folder to match the input filename defined in the code.

### 4. Compile the Program
- Navigate to the **Color_Affection** directory in Terminal.
- Compile the program with the following command:
  ```bash
  gcc main.c -lsndfile -o ColorAffection
  ```

### 5. Input RGB Values
- Run the compiled program by entering:
  ```bash
  ./ColorAffection
  ```
- Follow Terminal's prompt to sequentially enter **R**, **G**, and **B** values.
  - **Tip:** Use an online RGB-to-HSV converter to understand the effect.
  - **Alternative:** Experiment with colors in a color wheel for different audio transformations.
  - **Note:** Ensure RGB values are between `0` and `255`.

### 6. View Conversion Details
- Terminal will display the converted **HSV** values and their impact on **Delay**, **Decay**, and **Threshold**.

### 7. Access the Output File
- The transformed audio file is saved as **HSV1.wav** in the **Color_Affection** folder.
- To change the output filename, modify `OUTPUT_FILE_NAME` in the source code.

### 8. Final Check
- The modified audio file **HSV1.wav** will be saved automatically in the **Color_Affection** folder.

You’re now ready to create your own color-influenced audio files using Color-Affection!

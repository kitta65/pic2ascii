# %%
from PIL import Image
import numpy as np
from skimage.metrics import structural_similarity

# %%
names = ["o", "large_o", "slash", "pipe", "me"]
images = {n: Image.open(f"../input/{n}.png").convert("L") for n in names}
arrays = {n: np.array(images[n], dtype=np.uint8) for n in names}

# %%
for n1 in names:
    print(f"----- {n1} -----")
    for n2 in names:
        ssim = structural_similarity(arrays[n1], arrays[n2], win_size=33)
        print(f"{n2}: { ssim }")

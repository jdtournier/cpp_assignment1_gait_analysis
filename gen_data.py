#!/usr/bin/python

import numpy as np
import sys


def generate_robust_gait(filename, base_samples_per_step=95, scale=1, scale_shin=1, fs=100, num_steps=8):

    with open(filename, "w") as f:
        f.write("THIGH_ANG SHIN_ANG THIGH_ACCEL VERT_ACCEL\n");

        rand_scale = scale * np.random.normal (1,0.05,1)

        for i in range(num_steps):
            # Add variation to step duration
            step_len = int(95 * np.random.uniform(0.9, 1.1))
            t = np.linspace(0, 1, step_len, endpoint=False)

            # --- BIOMEDICAL MODELING ---
            # Thigh: Forward swing (+) followed by a controlled retraction
            t_ang = scale*(15 * np.sin(2 * np.pi * t) +
                     5 * np.sin(4 * np.pi * t) + 5)

            # Shin: Complex motion to create the double-hump knee flexion
            # We add a 2nd harmonic to create the 'Loading Response' dip
            s_ang = scale_shin*scale*(30 * np.sin(2 * np.pi * t - 0.8) +
                    20 * np.sin(4 * np.pi * t - 0.9) - 16)

            # --- ACCELERATION MODELING ---
            # Thigh Accel: Peaks during mid-swing (centripetal force)
            t_accel = 1.0 + 0.4 * scale * np.sin(np.pi * t)**2 + np.random.normal(0, 0.02, step_len)

            # Vert Accel: Sharp impact spike at the end of the step (Heel Strike)
            v_accel = 1.0 + np.random.normal(0, 0.03, step_len)
            v_accel[(0.25<t) & (t<0.32)] += scale*np.random.uniform(0.8, 1.4)

            t_ang += np.random.normal(0, 0.4, len(t_ang))
            s_ang += np.random.normal(0, 0.4, len(s_ang))

            prev_scale = rand_scale
            rand_scale = scale * np.random.normal (1,0.05,1)

            sc = np.linspace (prev_scale, rand_scale, t_ang.shape[0]).flatten()
            t_ang *= sc;
            s_ang *= sc;

            for t,s,a,v in zip (t_ang,s_ang,t_accel,v_accel):
              f.write (str(np.round(t,2)) + " " +
                       str(np.round(s,2)) + " " +
                       str(np.round(a,2)) + " " +
                       str(np.round(v,2)) + "\n")

    print(f"File '{filename}' generated")

generate_robust_gait(sys.argv[1], scale=0.9, scale_shin=0.8, base_samples_per_step=125)

try:
    import tensorflow as tf
    converter = tf.lite.TFLiteConverter.from_saved_model(r'C:\Users\shubh\EmbeddedML\Miniprojects\mp4\xyzModel')
    tflite_model = converter.convert()
    tflite_model_path = r"C:\Users\shubh\EmbeddedML\Miniprojects\mp4\xyzLiteModel"
    # tf.io.write_file(tflite_model_path, tflite_model)
    with open('xyzModel.tflite', 'wb') as f:
        f.write(tflite_model)
    print("Module Converted and Saved")
except:
    print("Error Converting Model")
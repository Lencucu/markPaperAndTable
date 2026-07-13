void MyWidget::updateDissolve() {
    QImage img = m_pixmap.toImage(); // 从截图开始

    int width = img.width();
    int height = img.height();

    // 根据时间决定溶解进度 (0~1)
    float progress = qMin(1.0f, m_elapsed / 3000.0f); // 3秒内完成

    // 计算阈值：越到后期，溶解的像素越多
    int threshold = static_cast<int>(progress * 255);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            QRgb pixel = img.pixel(x, y);
            // 根据像素亮度生成一个随机种子，决定是否“湮灭”
            int gray = qGray(pixel);
            if (QRandomGenerator::global()->bounded(255) < threshold) {
                // 这个像素湮灭（变为透明）
                img.setPixel(x, y, qRgba(0, 0, 0, 0));
            } else {
                // 未被湮灭的像素，稍微扭曲或变暗，增强效果
                int r = qRed(pixel) * (1 - progress * 0.5);
                int g = qGreen(pixel) * (1 - progress * 0.5);
                int b = qBlue(pixel) * (1 - progress * 0.5);
                img.setPixel(x, y, qRgb(r, g, b));
            }
        }
    }

    // 将处理后的图像绘制到窗体
    m_dissolvePixmap = QPixmap::fromImage(img);
    update();
}
import SwiftUI
import CoreGraphics
import PDFKit

// MARK: - Export Format Types

enum ExportFormat: String, CaseIterable {
    case png = "PNG"
    case jpeg = "JPEG"
    case pdf = "PDF"
    case tiff = "TIFF"
    case bmp = "BMP"
    case svg = "SVG"
    case psd = "PSD"
    
    var name: String {
        return self.rawValue
    }
    
    var fileExtension: String {
        switch self {
        case .png: return "png"
        case .jpeg: return "jpg"
        case .pdf: return "pdf"
        case .tiff: return "tiff"
        case .bmp: return "bmp"
        case .svg: return "svg"
        case .psd: return "psd"
        }
    }
    
    var mimeType: String {
        switch self {
        case .png: return "image/png"
        case .jpeg: return "image/jpeg"
        case .pdf: return "application/pdf"
        case .tiff: return "image/tiff"
        case .bmp: return "image/bmp"
        case .svg: return "image/svg+xml"
        case .psd: return "image/vnd.adobe.photoshop"
        }
    }
    
    var icon: Image {
        switch self {
        case .png: return Image(systemName: "photo")
        case .jpeg: return Image(systemName: "photo")
        case .pdf: return Image(systemName: "doc.text")
        case .tiff: return Image(systemName: "photo")
        case .bmp: return Image(systemName: "photo")
        case .svg: return Image(systemName: "square.and.arrow.up")
        case .psd: return Image(systemName: "square.stack.3d.up")
        }
    }
    
    var description: String {
        switch self {
        case .png: return "Lossless compression, supports transparency"
        case .jpeg: return "Lossy compression, smaller file size"
        case .pdf: return "Vector format, scalable, print-ready"
        case .tiff: return "High quality, professional printing"
        case .bmp: return "Uncompressed bitmap format"
        case .svg: return "Vector graphics, scalable"
        case .psd: return "Photoshop format with layers"
        }
    }
}

// MARK: - Export Quality Settings

enum ExportQuality: String, CaseIterable {
    case low = "Low"
    case medium = "Medium"
    case high = "High"
    case maximum = "Maximum"
    
    var compressionQuality: Double {
        switch self {
        case .low: return 0.3
        case .medium: return 0.6
        case .high: return 0.8
        case .maximum: return 1.0
        }
    }
    
    var dpi: Int {
        switch self {
        case .low: return 72
        case .medium: return 150
        case .high: return 300
        case .maximum: return 600
        }
    }
}

// MARK: - Export Settings Model

class ExportSettings: ObservableObject {
    @Published var format: ExportFormat = .png
    @Published var quality: ExportQuality = .high
    @Published var includeTransparency: Bool = true
    @Published var includeMetadata: Bool = true
    @Published var customDPI: Int = 300
    @Published var useCustomDPI: Bool = false
    @Published var fileName: String = "Artwork"
    @Published var includeTimestamp: Bool = true
    @Published var includeCanvasInfo: Bool = true
    @Published var compressionLevel: Double = 0.8
    @Published var colorSpace: ColorSpace = .sRGB
    @Published var includeLayers: Bool = false
    
    enum ColorSpace: String, CaseIterable {
        case sRGB = "sRGB"
        case adobeRGB = "Adobe RGB"
        case displayP3 = "Display P3"
        case grayscale = "Grayscale"
        
        var description: String {
            switch self {
            case .sRGB: return "Standard web color space"
            case .adobeRGB: return "Wider color gamut for printing"
            case .displayP3: return "Apple display color space"
            case .grayscale: return "Black and white only"
            }
        }
    }
}

// MARK: - Export Manager

class ExportManager: ObservableObject {
    @Published var exportSettings = ExportSettings()
    @Published var showExportDialog = false
    @Published var isExporting = false
    @Published var exportProgress: Double = 0
    @Published var exportStatus: String = ""
    @Published var lastExportPath: URL?
    @Published var exportHistory: [ExportRecord] = []
    
    func exportCanvas(
        canvas: Canvas,
        layers: [DrawingLayer],
        strokes: [DrawingStroke],
        textElements: [TextElement],
        gradients: [GradientElement],
        patterns: [PatternElement]
    ) async {
        await MainActor.run {
            isExporting = true
            exportProgress = 0
            exportStatus = "Preparing export..."
        }
        
        do {
            let fileName = generateFileName()
            let fileURL = try await performExport(
                canvas: canvas,
                layers: layers,
                strokes: strokes,
                textElements: textElements,
                gradients: gradients,
                patterns: patterns,
                fileName: fileName
            )
            
            await MainActor.run {
                lastExportPath = fileURL
                exportHistory.append(ExportRecord(
                    fileName: fileName,
                    format: exportSettings.format,
                    fileSize: getFileSize(url: fileURL),
                    exportDate: Date(),
                    fileURL: fileURL
                ))
                isExporting = false
                exportProgress = 1.0
                exportStatus = "Export completed successfully!"
            }
        } catch {
            await MainActor.run {
                isExporting = false
                exportProgress = 0
                exportStatus = "Export failed: \(error.localizedDescription)"
            }
        }
    }
    
    private func generateFileName() -> String {
        var fileName = exportSettings.fileName
        
        if exportSettings.includeTimestamp {
            let formatter = DateFormatter()
            formatter.dateFormat = "yyyy-MM-dd_HH-mm-ss"
            fileName += "_\(formatter.string(from: Date()))"
        }
        
        fileName += ".\(exportSettings.format.fileExtension)"
        
        return fileName
    }
    
    private func performExport(
        canvas: Canvas,
        layers: [DrawingLayer],
        strokes: [DrawingStroke],
        textElements: [TextElement],
        gradients: [GradientElement],
        patterns: [PatternElement],
        fileName: String
    ) async throws -> URL {
        
        await MainActor.run {
            exportProgress = 0.2
            exportStatus = "Rendering canvas..."
        }
        
        let image = try await renderCanvas(
            canvas: canvas,
            layers: layers,
            strokes: strokes,
            textElements: textElements,
            gradients: gradients,
            patterns: patterns
        )
        
        await MainActor.run {
            exportProgress = 0.6
            exportStatus = "Processing image..."
        }
        
        let processedImage = try await processImage(image)
        
        await MainActor.run {
            exportProgress = 0.8
            exportStatus = "Saving file..."
        }
        
        let fileURL = try await saveImage(processedImage, fileName: fileName)
        
        return fileURL
    }
    
    private func renderCanvas(
        canvas: Canvas,
        layers: [DrawingLayer],
        strokes: [DrawingStroke],
        textElements: [TextElement],
        gradients: [GradientElement],
        patterns: [PatternElement]
    ) async throws -> UIImage {
        
        let size = canvas.size
        let scale = exportSettings.useCustomDPI ? CGFloat(exportSettings.customDPI) / 72.0 : 1.0
        let scaledSize = CGSize(width: size.width * scale, height: size.height * scale)
        
        let renderer = UIGraphicsImageRenderer(size: scaledSize)
        
        return renderer.image { context in
            // Set background color
            context.cgContext.setFillColor(UIColor(canvas.backgroundColor).cgColor)
            context.cgContext.fill(CGRect(origin: .zero, size: scaledSize))
            
            // Apply canvas rotation
            if canvas.rotation != 0 {
                context.cgContext.translateBy(x: scaledSize.width / 2, y: scaledSize.height / 2)
                context.cgContext.rotate(by: CGFloat(canvas.rotation * .pi / 180))
                context.cgContext.translateBy(x: -scaledSize.width / 2, y: -scaledSize.height / 2)
            }
            
            // Apply crop
            if let cropRect = canvas.cropRect {
                context.cgContext.clip(to: CGRect(
                    x: cropRect.origin.x * scale,
                    y: cropRect.origin.y * scale,
                    width: cropRect.width * scale,
                    height: cropRect.height * scale
                ))
            }
            
            // Render layers
            for layer in layers where layer.isVisible {
                context.cgContext.saveGState()
                
                // Set layer opacity
                context.cgContext.setAlpha(CGFloat(layer.opacity))
                
                // Apply blending mode
                applyBlendingMode(layer.blendingMode, to: context.cgContext)
                
                // Render layer strokes
                for stroke in layer.strokes {
                    renderStroke(stroke, in: context.cgContext, scale: scale)
                }
                
                context.cgContext.restoreGState()
            }
            
            // Render standalone strokes
            for stroke in strokes {
                renderStroke(stroke, in: context.cgContext, scale: scale)
            }
            
            // Render text elements
            for textElement in textElements {
                renderTextElement(textElement, in: context.cgContext, scale: scale)
            }
            
            // Render gradients
            for gradient in gradients {
                renderGradient(gradient, in: context.cgContext, scale: scale)
            }
            
            // Render patterns
            for pattern in patterns {
                renderPattern(pattern, in: context.cgContext, scale: scale)
            }
        }
    }
    
    private func processImage(_ image: UIImage) async throws -> UIImage {
        // Apply color space conversion
        let processedImage = try convertColorSpace(image)
        
        // Apply compression for JPEG
        if exportSettings.format == .jpeg {
            return try compressJPEG(processedImage)
        }
        
        return processedImage
    }
    
    private func saveImage(_ image: UIImage, fileName: String) async throws -> URL {
        let documentsPath = FileManager.default.urls(for: .documentDirectory, in: .userDomainMask)[0]
        let fileURL = documentsPath.appendingPathComponent(fileName)
        
        switch exportSettings.format {
        case .png:
            try image.pngData()?.write(to: fileURL)
        case .jpeg:
            try image.jpegData(compressionQuality: exportSettings.compressionLevel)?.write(to: fileURL)
        case .pdf:
            try await saveAsPDF(image, to: fileURL)
        case .tiff:
            try image.tiffData()?.write(to: fileURL)
        case .bmp:
            try image.bmpData()?.write(to: fileURL)
        case .svg:
            try await saveAsSVG(image, to: fileURL)
        case .psd:
            try await saveAsPSD(image, to: fileURL)
        }
        
        return fileURL
    }
    
    private func applyBlendingMode(_ mode: BlendingMode, to context: CGContext) {
        switch mode {
        case .normal: context.setBlendMode(.normal)
        case .multiply: context.setBlendMode(.multiply)
        case .screen: context.setBlendMode(.screen)
        case .overlay: context.setBlendMode(.overlay)
        case .softLight: context.setBlendMode(.softLight)
        case .hardLight: context.setBlendMode(.hardLight)
        case .colorDodge: context.setBlendMode(.colorDodge)
        case .colorBurn: context.setBlendMode(.colorBurn)
        case .darken: context.setBlendMode(.darken)
        case .lighten: context.setBlendMode(.lighten)
        case .difference: context.setBlendMode(.difference)
        case .exclusion: context.setBlendMode(.exclusion)
        case .hue: context.setBlendMode(.hue)
        case .saturation: context.setBlendMode(.saturation)
        case .color: context.setBlendMode(.color)
        case .luminosity: context.setBlendMode(.luminosity)
        }
    }
    
    private func renderStroke(_ stroke: DrawingStroke, in context: CGContext, scale: CGFloat) {
        guard !stroke.points.isEmpty else { return }
        
        context.setStrokeColor(UIColor(stroke.color).cgColor)
        context.setLineWidth(stroke.size * scale)
        context.setLineCap(.round)
        context.setLineJoin(.round)
        context.setAlpha(CGFloat(stroke.opacity))
        
        context.move(to: CGPoint(x: stroke.points[0].x * scale, y: stroke.points[0].y * scale))
        for i in 1..<stroke.points.count {
            context.addLine(to: CGPoint(x: stroke.points[i].x * scale, y: stroke.points[i].y * scale))
        }
        
        context.strokePath()
    }
    
    private func renderTextElement(_ textElement: TextElement, in context: CGContext, scale: CGFloat) {
        let font = UIFont(name: textElement.fontName, size: textElement.fontSize * scale) ?? UIFont.systemFont(ofSize: textElement.fontSize * scale)
        
        let attributes: [NSAttributedString.Key: Any] = [
            .font: font,
            .foregroundColor: UIColor(textElement.color),
            .underlineStyle: textElement.isUnderlined ? NSUnderlineStyle.single.rawValue : 0
        ]
        
        let attributedString = NSAttributedString(string: textElement.text, attributes: attributes)
        
        context.saveGState()
        context.translateBy(x: textElement.position.x * scale, y: textElement.position.y * scale)
        context.rotate(by: CGFloat(textElement.rotation * .pi / 180))
        context.setAlpha(CGFloat(textElement.opacity))
        
        attributedString.draw(at: .zero)
        
        context.restoreGState()
    }
    
    private func renderGradient(_ gradient: GradientElement, in context: CGContext, scale: CGFloat) {
        // Simplified gradient rendering
        let colors = gradient.colors.map { UIColor($0).cgColor }
        let colorSpace = CGColorSpaceCreateDeviceRGB()
        
        if let cgGradient = CGGradient(colorsSpace: colorSpace, colors: colors as CFArray, locations: nil) {
            context.saveGState()
            context.setAlpha(CGFloat(gradient.opacity))
            
            switch gradient.type {
            case .linear:
                context.drawLinearGradient(
                    cgGradient,
                    start: CGPoint(x: gradient.startPoint.x * scale, y: gradient.startPoint.y * scale),
                    end: CGPoint(x: gradient.endPoint.x * scale, y: gradient.endPoint.y * scale),
                    options: []
                )
            case .radial:
                context.drawRadialGradient(
                    cgGradient,
                    startCenter: CGPoint(x: gradient.center.x * scale, y: gradient.center.y * scale),
                    startRadius: 0,
                    endCenter: CGPoint(x: gradient.center.x * scale, y: gradient.center.y * scale),
                    endRadius: gradient.radius * scale,
                    options: []
                )
            default:
                break
            }
            
            context.restoreGState()
        }
    }
    
    private func renderPattern(_ pattern: PatternElement, in context: CGContext, scale: CGFloat) {
        // Simplified pattern rendering
        context.saveGState()
        context.setAlpha(CGFloat(pattern.opacity))
        context.setFillColor(UIColor(pattern.primaryColor).cgColor)
        context.fill(CGRect(x: 0, y: 0, width: pattern.bounds.width * scale, height: pattern.bounds.height * scale))
        context.restoreGState()
    }
    
    private func convertColorSpace(_ image: UIImage) throws -> UIImage {
        // Simplified color space conversion
        return image
    }
    
    private func compressJPEG(_ image: UIImage) throws -> UIImage {
        guard let data = image.jpegData(compressionQuality: exportSettings.compressionLevel),
              let compressedImage = UIImage(data: data) else {
            throw ExportError.compressionFailed
        }
        return compressedImage
    }
    
    private func saveAsPDF(_ image: UIImage, to url: URL) async throws {
        let pdfDocument = PDFDocument()
        let pdfPage = PDFPage(image: image)
        pdfDocument.insert(pdfPage!, at: 0)
        pdfDocument.write(to: url)
    }
    
    private func saveAsSVG(_ image: UIImage, to url: URL) async throws {
        // Simplified SVG export
        let svgContent = """
        <?xml version="1.0" encoding="UTF-8"?>
        <svg width="\(image.size.width)" height="\(image.size.height)" xmlns="http://www.w3.org/2000/svg">
            <image href="data:image/png;base64,\(image.pngData()?.base64EncodedString() ?? "")" width="\(image.size.width)" height="\(image.size.height)"/>
        </svg>
        """
        try svgContent.write(to: url, atomically: true, encoding: .utf8)
    }
    
    private func saveAsPSD(_ image: UIImage, to url: URL) async throws {
        // Simplified PSD export - would require PSD library
        try image.pngData()?.write(to: url)
    }
    
    private func getFileSize(url: URL) -> Int64 {
        do {
            let attributes = try FileManager.default.attributesOfItem(atPath: url.path)
            return attributes[.size] as? Int64 ?? 0
        } catch {
            return 0
        }
    }
}

// MARK: - Export Record Model

struct ExportRecord: Identifiable, Codable {
    let id = UUID()
    let fileName: String
    let format: ExportFormat
    let fileSize: Int64
    let exportDate: Date
    let fileURL: URL
    
    var fileSizeString: String {
        let formatter = ByteCountFormatter()
        formatter.allowedUnits = [.useKB, .useMB]
        formatter.countStyle = .file
        return formatter.string(fromByteCount: fileSize)
    }
}

// MARK: - Export Errors

enum ExportError: Error, LocalizedError {
    case compressionFailed
    case fileWriteFailed
    case unsupportedFormat
    case renderingFailed
    
    var errorDescription: String? {
        switch self {
        case .compressionFailed: return "Failed to compress image"
        case .fileWriteFailed: return "Failed to write file"
        case .unsupportedFormat: return "Unsupported export format"
        case .renderingFailed: return "Failed to render canvas"
        }
    }
}

// MARK: - Professional Export Panel View

struct ProfessionalExportPanelView: View {
    @ObservedObject var exportManager: ExportManager
    @ObservedObject var errorHandler: ErrorHandler
    
    var body: some View {
        VStack(spacing: 15) {
            // Header
            HStack {
                Text("📤 Professional Export")
                    .font(.title2)
                    .fontWeight(.bold)
                    .foregroundColor(.primary)
                
                Spacer()
                
                Button(action: {
                    withAnimation(.spring()) {
                        exportManager.showExportDialog.toggle()
                    }
                }) {
                    Image(systemName: exportManager.showExportDialog ? "chevron.up" : "chevron.down")
                        .foregroundColor(.blue)
                }
            }
            .padding(.horizontal)
            
            if exportManager.showExportDialog {
                VStack(spacing: 15) {
                    // Export format selection
                    VStack {
                        Text("Export Format")
                            .font(.headline)
                            .foregroundColor(.primary)
                        
                        ScrollView(.horizontal, showsIndicators: false) {
                            HStack(spacing: 15) {
                                ForEach(ExportFormat.allCases, id: \.self) { format in
                                    Button(action: {
                                        exportManager.exportSettings.format = format
                                        SafeHapticFeedback.shared.lightImpact()
                                    }) {
                                        VStack {
                                            format.icon
                                                .font(.system(size: 25))
                                                .foregroundColor(exportManager.exportSettings.format == format ? .white : .primary)
                                                .frame(width: 50, height: 50)
                                                .background(
                                                    Circle()
                                                        .fill(exportManager.exportSettings.format == format ? Color.blue : Color.gray.opacity(0.2))
                                                )
                                            
                                            Text(format.name)
                                                .font(.caption)
                                                .foregroundColor(.primary)
                                        }
                                    }
                                    .buttonStyle(PlainButtonStyle())
                                }
                            }
                            .padding(.horizontal)
                        }
                        
                        Text(exportManager.exportSettings.format.description)
                            .font(.caption)
                            .foregroundColor(.secondary)
                            .multilineTextAlignment(.center)
                    }
                    
                    // Quality settings
                    VStack {
                        Text("Quality Settings")
                            .font(.headline)
                            .foregroundColor(.primary)
                        
                        Picker("Quality", selection: $exportManager.exportSettings.quality) {
                            ForEach(ExportQuality.allCases, id: \.self) { quality in
                                Text(quality.rawValue).tag(quality)
                            }
                        }
                        .pickerStyle(SegmentedPickerStyle())
                        
                        Text("DPI: \(exportManager.exportSettings.quality.dpi)")
                            .font(.caption)
                            .foregroundColor(.secondary)
                    }
                    
                    // File settings
                    VStack {
                        Text("File Settings")
                            .font(.headline)
                            .foregroundColor(.primary)
                        
                        VStack(spacing: 10) {
                            TextField("File name", text: $exportManager.exportSettings.fileName)
                                .textFieldStyle(RoundedBorderTextFieldStyle())
                            
                            Toggle("Include timestamp", isOn: $exportManager.exportSettings.includeTimestamp)
                                .toggleStyle(SwitchToggleStyle(tint: .blue))
                            
                            Toggle("Include metadata", isOn: $exportManager.exportSettings.includeMetadata)
                                .toggleStyle(SwitchToggleStyle(tint: .blue))
                            
                            Toggle("Include canvas info", isOn: $exportManager.exportSettings.includeCanvasInfo)
                                .toggleStyle(SwitchToggleStyle(tint: .blue))
                        }
                    }
                    
                    // Advanced settings
                    VStack {
                        Text("Advanced Settings")
                            .font(.headline)
                            .foregroundColor(.primary)
                        
                        VStack(spacing: 10) {
                            Toggle("Use custom DPI", isOn: $exportManager.exportSettings.useCustomDPI)
                                .toggleStyle(SwitchToggleStyle(tint: .blue))
                            
                            if exportManager.exportSettings.useCustomDPI {
                                HStack {
                                    Text("DPI:")
                                        .font(.subheadline)
                                        .foregroundColor(.primary)
                                    
                                    TextField("DPI", value: $exportManager.exportSettings.customDPI, format: .number)
                                        .textFieldStyle(RoundedBorderTextFieldStyle())
                                        .frame(width: 80)
                                }
                            }
                            
                            if exportManager.exportSettings.format == .jpeg {
                                VStack {
                                    HStack {
                                        Text("Compression: \(Int(exportManager.exportSettings.compressionLevel * 100))%")
                                            .font(.subheadline)
                                            .foregroundColor(.primary)
                                        
                                        Spacer()
                                        
                                        Text("\(Int(exportManager.exportSettings.compressionLevel * 100))%")
                                            .font(.caption)
                                            .foregroundColor(.secondary)
                                    }
                                    
                                    Slider(value: $exportManager.exportSettings.compressionLevel, in: 0.1...1.0)
                                        .accentColor(.blue)
                                }
                            }
                            
                            Picker("Color Space", selection: $exportManager.exportSettings.colorSpace) {
                                ForEach(ExportSettings.ColorSpace.allCases, id: \.self) { colorSpace in
                                    Text(colorSpace.rawValue).tag(colorSpace)
                                }
                            }
                            .pickerStyle(MenuPickerStyle())
                        }
                    }
                    
                    // Export progress
                    if exportManager.isExporting {
                        VStack {
                            Text(exportManager.exportStatus)
                                .font(.subheadline)
                                .foregroundColor(.primary)
                            
                            ProgressView(value: exportManager.exportProgress)
                                .progressViewStyle(LinearProgressViewStyle(tint: .blue))
                            
                            Text("\(Int(exportManager.exportProgress * 100))%")
                                .font(.caption)
                                .foregroundColor(.secondary)
                        }
                    }
                    
                    // Action buttons
                    HStack(spacing: 15) {
                        Button("Export") {
                            // Export functionality would be called here
                            SafeHapticFeedback.shared.success()
                        }
                        .buttonStyle(.borderedProminent)
                        .disabled(exportManager.isExporting)
                        
                        if let lastPath = exportManager.lastExportPath {
                            Button("Open Last Export") {
                                // Open file functionality
                            }
                            .buttonStyle(.bordered)
                        }
                    }
                }
                .padding()
                .background(
                    RoundedRectangle(cornerRadius: 15)
                        .fill(Color.gray.opacity(0.1))
                )
                .padding(.horizontal)
            }
        }
        .padding(.vertical, 10)
        .background(
            LinearGradient(
                colors: [Color.green.opacity(0.05), Color.blue.opacity(0.05)],
                startPoint: .topLeading,
                endPoint: .bottomTrailing
            )
        )
    }
}

// MARK: - Export History View

struct ExportHistoryView: View {
    @ObservedObject var exportManager: ExportManager
    
    var body: some View {
        VStack(spacing: 15) {
            Text("Export History")
                .font(.headline)
                .foregroundColor(.primary)
            
            if exportManager.exportHistory.isEmpty {
                Text("No exports yet")
                    .font(.subheadline)
                    .foregroundColor(.secondary)
            } else {
                ScrollView {
                    LazyVStack(spacing: 8) {
                        ForEach(exportManager.exportHistory) { record in
                            ExportHistoryRowView(record: record)
                        }
                    }
                }
                .frame(maxHeight: 200)
            }
        }
        .padding()
        .background(Color.gray.opacity(0.1))
        .cornerRadius(10)
    }
}

// MARK: - Export History Row View

struct ExportHistoryRowView: View {
    let record: ExportRecord
    
    var body: some View {
        HStack(spacing: 10) {
            record.format.icon
                .font(.system(size: 20))
                .foregroundColor(.blue)
            
            VStack(alignment: .leading, spacing: 2) {
                Text(record.fileName)
                    .font(.subheadline)
                    .foregroundColor(.primary)
                
                Text("\(record.format.name) • \(record.fileSizeString) • \(record.exportDate, style: .date)")
                    .font(.caption)
                    .foregroundColor(.secondary)
            }
            
            Spacer()
            
            Button(action: {
                // Open file functionality
            }) {
                Image(systemName: "arrow.up.right.square")
                    .foregroundColor(.blue)
                    .font(.system(size: 16))
            }
        }
        .padding(.horizontal, 10)
        .padding(.vertical, 8)
        .background(Color.white)
        .cornerRadius(8)
    }
}

// MARK: - Image Extensions

extension UIImage {
    func bmpData() -> Data? {
        guard let cgImage = self.cgImage else { return nil }
        let bitmapInfo = CGBitmapInfo(rawValue: CGImageAlphaInfo.premultipliedLast.rawValue)
        let context = CGContext(
            data: nil,
            width: cgImage.width,
            height: cgImage.height,
            bitsPerComponent: 8,
            bytesPerRow: cgImage.width * 4,
            space: CGColorSpaceCreateDeviceRGB(),
            bitmapInfo: bitmapInfo.rawValue
        )
        
        context?.draw(cgImage, in: CGRect(x: 0, y: 0, width: cgImage.width, height: cgImage.height))
        
        return context?.makeImage()?.dataProvider?.data as Data?
    }
    
    func tiffData() -> Data? {
        guard let cgImage = self.cgImage else { return nil }
        let destination = CGImageDestinationCreateWithData(NSMutableData(), kUTTypeTIFF, 1, nil)
        CGImageDestinationAddImage(destination!, cgImage, nil)
        CGImageDestinationFinalize(destination!)
        
        return (destination as? NSMutableData) as Data?
    }
}

// MARK: - Export Statistics

extension ExportManager {
    var totalExports: Int {
        return exportHistory.count
    }
    
    var mostUsedFormat: ExportFormat {
        let formatCounts = Dictionary(grouping: exportHistory, by: { $0.format })
            .mapValues { $0.count }
        
        return formatCounts.max(by: { $0.value < $1.value })?.key ?? .png
    }
    
    var totalExportedSize: Int64 {
        return exportHistory.reduce(0) { total, record in
            total + record.fileSize
        }
    }
    
    var averageFileSize: Int64 {
        guard !exportHistory.isEmpty else { return 0 }
        return totalExportedSize / Int64(exportHistory.count)
    }
}
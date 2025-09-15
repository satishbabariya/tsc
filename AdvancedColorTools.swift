import SwiftUI
import CoreGraphics

// MARK: - Color Tool Types

enum ColorTool: String, CaseIterable {
    case eyedropper = "eyedropper"
    case colorPicker = "color_picker"
    case colorHistory = "color_history"
    case colorPalette = "color_palette"
    case colorWheel = "color_wheel"
    case colorHarmony = "color_harmony"
    case colorBlindness = "color_blindness"
    case colorAnalyzer = "color_analyzer"
    
    var name: String {
        switch self {
        case .eyedropper: return "Eyedropper"
        case .colorPicker: return "Color Picker"
        case .colorHistory: return "Color History"
        case .colorPalette: return "Color Palette"
        case .colorWheel: return "Color Wheel"
        case .colorHarmony: return "Color Harmony"
        case .colorBlindness: return "Color Blindness"
        case .colorAnalyzer: return "Color Analyzer"
        }
    }
    
    var icon: Image {
        switch self {
        case .eyedropper: return Image(systemName: "eyedropper")
        case .colorPicker: return Image(systemName: "paintpalette")
        case .colorHistory: return Image(systemName: "clock")
        case .colorPalette: return Image(systemName: "square.grid.3x3")
        case .colorWheel: return Image(systemName: "circle")
        case .colorHarmony: return Image(systemName: "music.note")
        case .colorBlindness: return Image(systemName: "eye")
        case .colorAnalyzer: return Image(systemName: "chart.bar")
        }
    }
}

// MARK: - Color Sample Model

struct ColorSample: Identifiable, Codable {
    let id = UUID()
    let color: Color
    let position: CGPoint
    let timestamp: Date
    let hexValue: String
    let rgbValues: (red: Double, green: Double, blue: Double)
    let hsvValues: (hue: Double, saturation: Double, value: Double)
    
    init(color: Color, position: CGPoint = CGPoint.zero) {
        self.color = color
        self.position = position
        self.timestamp = Date()
        
        let rgb = color.toRGB()
        self.rgbValues = rgb
        self.hexValue = color.toHex()
        
        let hsv = color.toHSV()
        self.hsvValues = hsv
    }
}

// MARK: - Color Harmony Types

enum ColorHarmony: String, CaseIterable {
    case monochromatic = "monochromatic"
    case analogous = "analogous"
    case complementary = "complementary"
    case triadic = "triadic"
    case tetradic = "tetradic"
    case splitComplementary = "split_complementary"
    
    var name: String {
        switch self {
        case .monochromatic: return "Monochromatic"
        case .analogous: return "Analogous"
        case .complementary: return "Complementary"
        case .triadic: return "Triadic"
        case .tetradic: return "Tetradic"
        case .splitComplementary: return "Split Complementary"
        }
    }
    
    var description: String {
        switch self {
        case .monochromatic: return "Different shades of the same color"
        case .analogous: return "Colors next to each other on the color wheel"
        case .complementary: return "Colors opposite each other on the color wheel"
        case .triadic: return "Three colors evenly spaced on the color wheel"
        case .tetradic: return "Four colors forming a rectangle on the color wheel"
        case .splitComplementary: return "A color and the two colors adjacent to its complement"
        }
    }
}

// MARK: - Color Blindness Types

enum ColorBlindnessType: String, CaseIterable {
    case protanopia = "protanopia"
    case deuteranopia = "deuteranopia"
    case tritanopia = "tritanopia"
    case achromatopsia = "achromatopsia"
    
    var name: String {
        switch self {
        case .protanopia: return "Protanopia (Red-blind)"
        case .deuteranopia: return "Deuteranopia (Green-blind)"
        case .tritanopia: return "Tritanopia (Blue-blind)"
        case .achromatopsia: return "Achromatopsia (Complete color blindness)"
        }
    }
    
    var description: String {
        switch self {
        case .protanopia: return "Cannot see red light"
        case .deuteranopia: return "Cannot see green light"
        case .tritanopia: return "Cannot see blue light"
        case .achromatopsia: return "Cannot see any colors"
        }
    }
}

// MARK: - Advanced Color Manager

class AdvancedColorManager: ObservableObject {
    @Published var currentTool: ColorTool = .eyedropper
    @Published var selectedColor: Color = .blue
    @Published var colorHistory: [ColorSample] = []
    @Published var colorPalettes: [ColorPalette] = []
    @Published var showAdvancedColorPanel = false
    @Published var isEyedropperActive = false
    @Published var eyedropperPosition: CGPoint = .zero
    @Published var eyedropperColor: Color = .clear
    
    // Color analysis
    @Published var colorAnalysis: ColorAnalysis?
    @Published var selectedHarmony: ColorHarmony = .complementary
    @Published var harmonyColors: [Color] = []
    @Published var selectedBlindnessType: ColorBlindnessType = .protanopia
    @Published var blindnessSimulation: Color = .clear
    
    // Color picker settings
    @Published var hue: Double = 0
    @Published var saturation: Double = 1
    @Published var brightness: Double = 1
    @Published var alpha: Double = 1
    
    init() {
        loadColorHistory()
        loadColorPalettes()
    }
    
    func sampleColor(at position: CGPoint, from image: UIImage?) -> Color? {
        guard let image = image else { return nil }
        
        let color = extractColor(from: image, at: position)
        if let color = color {
            let sample = ColorSample(color: color, position: position)
            addToHistory(sample)
            return color
        }
        
        return nil
    }
    
    func addToHistory(_ sample: ColorSample) {
        colorHistory.insert(sample, at: 0)
        
        // Limit history to 50 colors
        if colorHistory.count > 50 {
            colorHistory.removeLast()
        }
        
        saveColorHistory()
    }
    
    func generateHarmonyColors(from baseColor: Color) {
        harmonyColors = ColorHarmonyEngine.generateHarmony(baseColor, type: selectedHarmony)
    }
    
    func simulateColorBlindness(_ color: Color, type: ColorBlindnessType) -> Color {
        return ColorBlindnessSimulator.simulate(color, type: type)
    }
    
    func analyzeColor(_ color: Color) {
        colorAnalysis = ColorAnalyzer.analyze(color)
    }
    
    func createColorPalette(name: String, colors: [Color]) {
        let palette = ColorPalette(name: name, colors: colors)
        colorPalettes.append(palette)
        saveColorPalettes()
    }
    
    private func extractColor(from image: UIImage, at position: CGPoint) -> Color? {
        guard let cgImage = image.cgImage else { return nil }
        
        let scale = image.scale
        let x = Int(position.x * scale)
        let y = Int(position.y * scale)
        
        guard x >= 0 && x < cgImage.width && y >= 0 && y < cgImage.height else { return nil }
        
        let pixelData = cgImage.dataProvider?.data
        let data = CFDataGetBytePtr(pixelData)
        
        let pixelIndex = (y * cgImage.width + x) * 4
        
        let r = CGFloat(data![pixelIndex]) / 255.0
        let g = CGFloat(data![pixelIndex + 1]) / 255.0
        let b = CGFloat(data![pixelIndex + 2]) / 255.0
        let a = CGFloat(data![pixelIndex + 3]) / 255.0
        
        return Color(red: r, green: g, blue: b, opacity: a)
    }
    
    private func saveColorHistory() {
        if let encoded = try? JSONEncoder().encode(colorHistory) {
            UserDefaults.standard.set(encoded, forKey: "colorHistory")
        }
    }
    
    private func loadColorHistory() {
        if let data = UserDefaults.standard.data(forKey: "colorHistory"),
           let decoded = try? JSONDecoder().decode([ColorSample].self, from: data) {
            colorHistory = decoded
        }
    }
    
    private func saveColorPalettes() {
        if let encoded = try? JSONEncoder().encode(colorPalettes) {
            UserDefaults.standard.set(encoded, forKey: "colorPalettes")
        }
    }
    
    private func loadColorPalettes() {
        if let data = UserDefaults.standard.data(forKey: "colorPalettes"),
           let decoded = try? JSONDecoder().decode([ColorPalette].self, from: data) {
            colorPalettes = decoded
        }
    }
}

// MARK: - Color Analysis Model

struct ColorAnalysis {
    let hexValue: String
    let rgbValues: (red: Int, green: Int, blue: Int)
    let hsvValues: (hue: Int, saturation: Int, value: Int)
    let hslValues: (hue: Int, saturation: Int, lightness: Int)
    let cmykValues: (cyan: Int, magenta: Int, yellow: Int, black: Int)
    let luminance: Double
    let temperature: ColorTemperature
    let psychologicalMeaning: String
    let accessibilityScore: Int
}

enum ColorTemperature: String, CaseIterable {
    case warm = "Warm"
    case cool = "Cool"
    case neutral = "Neutral"
}

// MARK: - Advanced Color Panel View

struct AdvancedColorPanelView: View {
    @ObservedObject var colorManager: AdvancedColorManager
    @ObservedObject var errorHandler: ErrorHandler
    
    var body: some View {
        VStack(spacing: 15) {
            // Header
            HStack {
                Text("🎨 Advanced Color Tools")
                    .font(.title2)
                    .fontWeight(.bold)
                    .foregroundColor(.primary)
                
                Spacer()
                
                Button(action: {
                    withAnimation(.spring()) {
                        colorManager.showAdvancedColorPanel.toggle()
                    }
                }) {
                    Image(systemName: colorManager.showAdvancedColorPanel ? "chevron.up" : "chevron.down")
                        .foregroundColor(.blue)
                }
            }
            .padding(.horizontal)
            
            if colorManager.showAdvancedColorPanel {
                VStack(spacing: 15) {
                    // Tool selector
                    ScrollView(.horizontal, showsIndicators: false) {
                        HStack(spacing: 15) {
                            ForEach(ColorTool.allCases, id: \.self) { tool in
                                Button(action: {
                                    colorManager.currentTool = tool
                                    SafeHapticFeedback.shared.lightImpact()
                                }) {
                                    VStack {
                                        tool.icon
                                            .font(.system(size: 25))
                                            .foregroundColor(colorManager.currentTool == tool ? .white : .primary)
                                            .frame(width: 50, height: 50)
                                            .background(
                                                Circle()
                                                    .fill(colorManager.currentTool == tool ? Color.blue : Color.gray.opacity(0.2))
                                            )
                                        
                                        Text(tool.name)
                                            .font(.caption)
                                            .foregroundColor(.primary)
                                    }
                                }
                                .buttonStyle(PlainButtonStyle())
                            }
                        }
                        .padding(.horizontal)
                    }
                    
                    // Tool-specific content
                    switch colorManager.currentTool {
                    case .eyedropper:
                        EyedropperView(colorManager: colorManager)
                    case .colorPicker:
                        AdvancedColorPickerView(colorManager: colorManager)
                    case .colorHistory:
                        ColorHistoryView(colorManager: colorManager)
                    case .colorPalette:
                        ColorPaletteManagerView(colorManager: colorManager)
                    case .colorWheel:
                        ColorWheelView(colorManager: colorManager)
                    case .colorHarmony:
                        ColorHarmonyView(colorManager: colorManager)
                    case .colorBlindness:
                        ColorBlindnessView(colorManager: colorManager)
                    case .colorAnalyzer:
                        ColorAnalyzerView(colorManager: colorManager)
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
                colors: [Color.blue.opacity(0.05), Color.purple.opacity(0.05)],
                startPoint: .topLeading,
                endPoint: .bottomTrailing
            )
        )
    }
}

// MARK: - Eyedropper View

struct EyedropperView: View {
    @ObservedObject var colorManager: AdvancedColorManager
    
    var body: some View {
        VStack(spacing: 15) {
            Text("Eyedropper Tool")
                .font(.headline)
                .foregroundColor(.primary)
            
            Text("Tap and drag to sample colors from the canvas")
                .font(.subheadline)
                .foregroundColor(.secondary)
                .multilineTextAlignment(.center)
            
            // Current sampled color
            VStack {
                Text("Sampled Color")
                    .font(.subheadline)
                    .foregroundColor(.primary)
                
                Circle()
                    .fill(colorManager.eyedropperColor)
                    .frame(width: 80, height: 80)
                    .overlay(
                        Circle()
                            .stroke(Color.black, lineWidth: 2)
                    )
                
                Text(colorManager.eyedropperColor.toHex())
                    .font(.caption)
                    .foregroundColor(.secondary)
            }
            
            // Eyedropper settings
            Toggle("Eyedropper Active", isOn: $colorManager.isEyedropperActive)
                .toggleStyle(SwitchToggleStyle(tint: .blue))
        }
    }
}

// MARK: - Advanced Color Picker View

struct AdvancedColorPickerView: View {
    @ObservedObject var colorManager: AdvancedColorManager
    
    var body: some View {
        VStack(spacing: 15) {
            Text("Advanced Color Picker")
                .font(.headline)
                .foregroundColor(.primary)
            
            // Color preview
            Circle()
                .fill(colorManager.selectedColor)
                .frame(width: 100, height: 100)
                .overlay(
                    Circle()
                        .stroke(Color.black, lineWidth: 3)
                )
            
            // HSV sliders
            VStack(spacing: 10) {
                VStack {
                    HStack {
                        Text("Hue: \(Int(colorManager.hue * 360))°")
                            .font(.subheadline)
                            .foregroundColor(.primary)
                        
                        Spacer()
                        
                        Text("\(Int(colorManager.hue * 360))°")
                            .font(.caption)
                            .foregroundColor(.secondary)
                    }
                    
                    Slider(value: $colorManager.hue, in: 0...1)
                        .accentColor(.red)
                }
                
                VStack {
                    HStack {
                        Text("Saturation: \(Int(colorManager.saturation * 100))%")
                            .font(.subheadline)
                            .foregroundColor(.primary)
                        
                        Spacer()
                        
                        Text("\(Int(colorManager.saturation * 100))%")
                            .font(.caption)
                            .foregroundColor(.secondary)
                    }
                    
                    Slider(value: $colorManager.saturation, in: 0...1)
                        .accentColor(.green)
                }
                
                VStack {
                    HStack {
                        Text("Brightness: \(Int(colorManager.brightness * 100))%")
                            .font(.subheadline)
                            .foregroundColor(.primary)
                        
                        Spacer()
                        
                        Text("\(Int(colorManager.brightness * 100))%")
                            .font(.caption)
                            .foregroundColor(.secondary)
                    }
                    
                    Slider(value: $colorManager.brightness, in: 0...1)
                        .accentColor(.blue)
                }
                
                VStack {
                    HStack {
                        Text("Alpha: \(Int(colorManager.alpha * 100))%")
                            .font(.subheadline)
                            .foregroundColor(.primary)
                        
                        Spacer()
                        
                        Text("\(Int(colorManager.alpha * 100))%")
                            .font(.caption)
                            .foregroundColor(.secondary)
                    }
                    
                    Slider(value: $colorManager.alpha, in: 0...1)
                        .accentColor(.gray)
                }
            }
            
            // Color values
            VStack {
                Text("Color Values")
                    .font(.subheadline)
                    .foregroundColor(.primary)
                
                VStack(spacing: 5) {
                    Text("HEX: \(colorManager.selectedColor.toHex())")
                        .font(.caption)
                        .foregroundColor(.secondary)
                    
                    let rgb = colorManager.selectedColor.toRGB()
                    Text("RGB: \(Int(rgb.red * 255)), \(Int(rgb.green * 255)), \(Int(rgb.blue * 255))")
                        .font(.caption)
                        .foregroundColor(.secondary)
                    
                    let hsv = colorManager.selectedColor.toHSV()
                    Text("HSV: \(Int(hsv.hue * 360))°, \(Int(hsv.saturation * 100))%, \(Int(hsv.value * 100))%")
                        .font(.caption)
                        .foregroundColor(.secondary)
                }
            }
        }
        .onChange(of: colorManager.hue) { _ in
            updateColorFromHSV()
        }
        .onChange(of: colorManager.saturation) { _ in
            updateColorFromHSV()
        }
        .onChange(of: colorManager.brightness) { _ in
            updateColorFromHSV()
        }
        .onChange(of: colorManager.alpha) { _ in
            updateColorFromHSV()
        }
    }
    
    private func updateColorFromHSV() {
        colorManager.selectedColor = Color.fromHSV(
            hue: colorManager.hue,
            saturation: colorManager.saturation,
            value: colorManager.brightness
        ).opacity(colorManager.alpha)
    }
}

// MARK: - Color History View

struct ColorHistoryView: View {
    @ObservedObject var colorManager: AdvancedColorManager
    
    var body: some View {
        VStack(spacing: 15) {
            Text("Color History")
                .font(.headline)
                .foregroundColor(.primary)
            
            if colorManager.colorHistory.isEmpty {
                Text("No colors sampled yet")
                    .font(.subheadline)
                    .foregroundColor(.secondary)
            } else {
                ScrollView {
                    LazyVGrid(columns: Array(repeating: GridItem(.flexible()), count: 8), spacing: 8) {
                        ForEach(colorManager.colorHistory) { sample in
                            Button(action: {
                                colorManager.selectedColor = sample.color
                            }) {
                                Circle()
                                    .fill(sample.color)
                                    .frame(width: 30, height: 30)
                                    .overlay(
                                        Circle()
                                            .stroke(Color.black, lineWidth: 1)
                                    )
                            }
                            .buttonStyle(PlainButtonStyle())
                        }
                    }
                }
                .frame(maxHeight: 200)
            }
            
            if !colorManager.colorHistory.isEmpty {
                Button("Clear History") {
                    colorManager.colorHistory.removeAll()
                }
                .buttonStyle(.bordered)
            }
        }
    }
}

// MARK: - Color Palette Manager View

struct ColorPaletteManagerView: View {
    @ObservedObject var colorManager: AdvancedColorManager
    @State private var showCreatePaletteDialog = false
    
    var body: some View {
        VStack(spacing: 15) {
            Text("Color Palettes")
                .font(.headline)
                .foregroundColor(.primary)
            
            if colorManager.colorPalettes.isEmpty {
                Text("No palettes created yet")
                    .font(.subheadline)
                    .foregroundColor(.secondary)
            } else {
                ScrollView {
                    LazyVStack(spacing: 10) {
                        ForEach(colorManager.colorPalettes) { palette in
                            ColorPaletteRowView(palette: palette, colorManager: colorManager)
                        }
                    }
                }
                .frame(maxHeight: 200)
            }
            
            Button("Create Palette") {
                showCreatePaletteDialog = true
            }
            .buttonStyle(.borderedProminent)
        }
        .sheet(isPresented: $showCreatePaletteDialog) {
            CreatePaletteDialogView(colorManager: colorManager)
        }
    }
}

// MARK: - Color Wheel View

struct ColorWheelView: View {
    @ObservedObject var colorManager: AdvancedColorManager
    
    var body: some View {
        VStack(spacing: 15) {
            Text("Color Wheel")
                .font(.headline)
                .foregroundColor(.primary)
            
            // Simplified color wheel representation
            ZStack {
                Circle()
                    .fill(
                        AngularGradient(
                            colors: [
                                .red, .orange, .yellow, .green,
                                .cyan, .blue, .purple, .pink, .red
                            ],
                            center: .center
                        )
                    )
                    .frame(width: 200, height: 200)
                
                Circle()
                    .fill(Color.white)
                    .frame(width: 100, height: 100)
                
                Circle()
                    .fill(colorManager.selectedColor)
                    .frame(width: 80, height: 80)
            }
            
            Text("Tap on the color wheel to select colors")
                .font(.caption)
                .foregroundColor(.secondary)
                .multilineTextAlignment(.center)
        }
    }
}

// MARK: - Color Harmony View

struct ColorHarmonyView: View {
    @ObservedObject var colorManager: AdvancedColorManager
    
    var body: some View {
        VStack(spacing: 15) {
            Text("Color Harmony")
                .font(.headline)
                .foregroundColor(.primary)
            
            // Harmony type selector
            Picker("Harmony Type", selection: $colorManager.selectedHarmony) {
                ForEach(ColorHarmony.allCases, id: \.self) { harmony in
                    Text(harmony.name).tag(harmony)
                }
            }
            .pickerStyle(MenuPickerStyle())
            .onChange(of: colorManager.selectedHarmony) { _ in
                colorManager.generateHarmonyColors(from: colorManager.selectedColor)
            }
            
            // Base color
            VStack {
                Text("Base Color")
                    .font(.subheadline)
                    .foregroundColor(.primary)
                
                Circle()
                    .fill(colorManager.selectedColor)
                    .frame(width: 60, height: 60)
                    .overlay(
                        Circle()
                            .stroke(Color.black, lineWidth: 2)
                    )
            }
            
            // Harmony colors
            if !colorManager.harmonyColors.isEmpty {
                VStack {
                    Text("Harmony Colors")
                        .font(.subheadline)
                        .foregroundColor(.primary)
                    
                    HStack(spacing: 10) {
                        ForEach(colorManager.harmonyColors, id: \.self) { color in
                            Button(action: {
                                colorManager.selectedColor = color
                            }) {
                                Circle()
                                    .fill(color)
                                    .frame(width: 40, height: 40)
                                    .overlay(
                                        Circle()
                                            .stroke(Color.black, lineWidth: 1)
                                    )
                            }
                            .buttonStyle(PlainButtonStyle())
                        }
                    }
                }
            }
            
            Button("Generate Harmony") {
                colorManager.generateHarmonyColors(from: colorManager.selectedColor)
            }
            .buttonStyle(.borderedProminent)
        }
    }
}

// MARK: - Color Blindness View

struct ColorBlindnessView: View {
    @ObservedObject var colorManager: AdvancedColorManager
    
    var body: some View {
        VStack(spacing: 15) {
            Text("Color Blindness Simulation")
                .font(.headline)
                .foregroundColor(.primary)
            
            // Blindness type selector
            Picker("Blindness Type", selection: $colorManager.selectedBlindnessType) {
                ForEach(ColorBlindnessType.allCases, id: \.self) { type in
                    Text(type.name).tag(type)
                }
            }
            .pickerStyle(MenuPickerStyle())
            
            // Original vs simulated
            HStack(spacing: 20) {
                VStack {
                    Text("Original")
                        .font(.subheadline)
                        .foregroundColor(.primary)
                    
                    Circle()
                        .fill(colorManager.selectedColor)
                        .frame(width: 80, height: 80)
                        .overlay(
                            Circle()
                                .stroke(Color.black, lineWidth: 2)
                        )
                }
                
                VStack {
                    Text("Simulated")
                        .font(.subheadline)
                        .foregroundColor(.primary)
                    
                    Circle()
                        .fill(colorManager.blindnessSimulation)
                        .frame(width: 80, height: 80)
                        .overlay(
                            Circle()
                                .stroke(Color.black, lineWidth: 2)
                        )
                }
            }
            
            Button("Simulate") {
                colorManager.blindnessSimulation = colorManager.simulateColorBlindness(
                    colorManager.selectedColor,
                    type: colorManager.selectedBlindnessType
                )
            }
            .buttonStyle(.borderedProminent)
        }
    }
}

// MARK: - Color Analyzer View

struct ColorAnalyzerView: View {
    @ObservedObject var colorManager: AdvancedColorManager
    
    var body: some View {
        VStack(spacing: 15) {
            Text("Color Analysis")
                .font(.headline)
                .foregroundColor(.primary)
            
            if let analysis = colorManager.colorAnalysis {
                VStack(spacing: 10) {
                    Text("HEX: \(analysis.hexValue)")
                        .font(.subheadline)
                        .foregroundColor(.primary)
                    
                    Text("RGB: \(analysis.rgbValues.red), \(analysis.rgbValues.green), \(analysis.rgbValues.blue)")
                        .font(.caption)
                        .foregroundColor(.secondary)
                    
                    Text("HSV: \(analysis.hsvValues.hue)°, \(analysis.hsvValues.saturation)%, \(analysis.hsvValues.value)%")
                        .font(.caption)
                        .foregroundColor(.secondary)
                    
                    Text("Temperature: \(analysis.temperature.rawValue)")
                        .font(.caption)
                        .foregroundColor(.secondary)
                    
                    Text("Luminance: \(Int(analysis.luminance * 100))%")
                        .font(.caption)
                        .foregroundColor(.secondary)
                    
                    Text("Accessibility Score: \(analysis.accessibilityScore)/100")
                        .font(.caption)
                        .foregroundColor(.secondary)
                    
                    Text(analysis.psychologicalMeaning)
                        .font(.caption)
                        .foregroundColor(.secondary)
                        .multilineTextAlignment(.center)
                }
            } else {
                Text("No analysis available")
                    .font(.subheadline)
                    .foregroundColor(.secondary)
            }
            
            Button("Analyze Color") {
                colorManager.analyzeColor(colorManager.selectedColor)
            }
            .buttonStyle(.borderedProminent)
        }
    }
}

// MARK: - Color Harmony Engine

class ColorHarmonyEngine {
    static func generateHarmony(_ baseColor: Color, type: ColorHarmony) -> [Color] {
        let hsv = baseColor.toHSV()
        let hue = hsv.hue
        let saturation = hsv.saturation
        let value = hsv.value
        
        switch type {
        case .monochromatic:
            return generateMonochromatic(hue: hue, saturation: saturation, value: value)
        case .analogous:
            return generateAnalogous(hue: hue, saturation: saturation, value: value)
        case .complementary:
            return generateComplementary(hue: hue, saturation: saturation, value: value)
        case .triadic:
            return generateTriadic(hue: hue, saturation: saturation, value: value)
        case .tetradic:
            return generateTetradic(hue: hue, saturation: saturation, value: value)
        case .splitComplementary:
            return generateSplitComplementary(hue: hue, saturation: saturation, value: value)
        }
    }
    
    private static func generateMonochromatic(hue: Double, saturation: Double, value: Double) -> [Color] {
        return [
            Color.fromHSV(hue: hue, saturation: saturation, value: max(0, value - 0.3)),
            Color.fromHSV(hue: hue, saturation: saturation, value: value),
            Color.fromHSV(hue: hue, saturation: saturation, value: min(1, value + 0.3))
        ]
    }
    
    private static func generateAnalogous(hue: Double, saturation: Double, value: Double) -> [Color] {
        return [
            Color.fromHSV(hue: fmod(hue - 0.1 + 1, 1), saturation: saturation, value: value),
            Color.fromHSV(hue: hue, saturation: saturation, value: value),
            Color.fromHSV(hue: fmod(hue + 0.1, 1), saturation: saturation, value: value)
        ]
    }
    
    private static func generateComplementary(hue: Double, saturation: Double, value: Double) -> [Color] {
        return [
            Color.fromHSV(hue: hue, saturation: saturation, value: value),
            Color.fromHSV(hue: fmod(hue + 0.5, 1), saturation: saturation, value: value)
        ]
    }
    
    private static func generateTriadic(hue: Double, saturation: Double, value: Double) -> [Color] {
        return [
            Color.fromHSV(hue: hue, saturation: saturation, value: value),
            Color.fromHSV(hue: fmod(hue + 0.33, 1), saturation: saturation, value: value),
            Color.fromHSV(hue: fmod(hue + 0.66, 1), saturation: saturation, value: value)
        ]
    }
    
    private static func generateTetradic(hue: Double, saturation: Double, value: Double) -> [Color] {
        return [
            Color.fromHSV(hue: hue, saturation: saturation, value: value),
            Color.fromHSV(hue: fmod(hue + 0.25, 1), saturation: saturation, value: value),
            Color.fromHSV(hue: fmod(hue + 0.5, 1), saturation: saturation, value: value),
            Color.fromHSV(hue: fmod(hue + 0.75, 1), saturation: saturation, value: value)
        ]
    }
    
    private static func generateSplitComplementary(hue: Double, saturation: Double, value: Double) -> [Color] {
        return [
            Color.fromHSV(hue: hue, saturation: saturation, value: value),
            Color.fromHSV(hue: fmod(hue + 0.4, 1), saturation: saturation, value: value),
            Color.fromHSV(hue: fmod(hue + 0.6, 1), saturation: saturation, value: value)
        ]
    }
}

// MARK: - Color Blindness Simulator

class ColorBlindnessSimulator {
    static func simulate(_ color: Color, type: ColorBlindnessType) -> Color {
        let rgb = color.toRGB()
        
        switch type {
        case .protanopia:
            return simulateProtanopia(rgb)
        case .deuteranopia:
            return simulateDeuteranopia(rgb)
        case .tritanopia:
            return simulateTritanopia(rgb)
        case .achromatopsia:
            return simulateAchromatopsia(rgb)
        }
    }
    
    private static func simulateProtanopia(_ rgb: (red: Double, green: Double, blue: Double)) -> Color {
        // Simplified protanopia simulation
        let gray = (rgb.red + rgb.green + rgb.blue) / 3
        return Color(red: gray, green: gray, blue: rgb.blue)
    }
    
    private static func simulateDeuteranopia(_ rgb: (red: Double, green: Double, blue: Double)) -> Color {
        // Simplified deuteranopia simulation
        let gray = (rgb.red + rgb.green + rgb.blue) / 3
        return Color(red: rgb.red, green: gray, blue: rgb.blue)
    }
    
    private static func simulateTritanopia(_ rgb: (red: Double, green: Double, blue: Double)) -> Color {
        // Simplified tritanopia simulation
        let gray = (rgb.red + rgb.green + rgb.blue) / 3
        return Color(red: rgb.red, green: rgb.green, blue: gray)
    }
    
    private static func simulateAchromatopsia(_ rgb: (red: Double, green: Double, blue: Double)) -> Color {
        // Complete color blindness - convert to grayscale
        let gray = (rgb.red + rgb.green + rgb.blue) / 3
        return Color(red: gray, green: gray, blue: gray)
    }
}

// MARK: - Color Analyzer

class ColorAnalyzer {
    static func analyze(_ color: Color) -> ColorAnalysis {
        let rgb = color.toRGB()
        let hsv = color.toHSV()
        let hsl = rgbToHSL(rgb)
        let cmyk = rgbToCMYK(rgb)
        let luminance = calculateLuminance(rgb)
        let temperature = determineTemperature(hsv)
        let psychologicalMeaning = getPsychologicalMeaning(hsv)
        let accessibilityScore = calculateAccessibilityScore(rgb)
        
        return ColorAnalysis(
            hexValue: color.toHex(),
            rgbValues: (Int(rgb.red * 255), Int(rgb.green * 255), Int(rgb.blue * 255)),
            hsvValues: (Int(hsv.hue * 360), Int(hsv.saturation * 100), Int(hsv.value * 100)),
            hslValues: (Int(hsl.hue * 360), Int(hsl.saturation * 100), Int(hsl.lightness * 100)),
            cmykValues: (Int(cmyk.cyan * 100), Int(cmyk.magenta * 100), Int(cmyk.yellow * 100), Int(cmyk.black * 100)),
            luminance: luminance,
            temperature: temperature,
            psychologicalMeaning: psychologicalMeaning,
            accessibilityScore: accessibilityScore
        )
    }
    
    private static func rgbToHSL(_ rgb: (red: Double, green: Double, blue: Double)) -> (hue: Double, saturation: Double, lightness: Double) {
        let max = max(rgb.red, max(rgb.green, rgb.blue))
        let min = min(rgb.red, min(rgb.green, rgb.blue))
        let delta = max - min
        
        let lightness = (max + min) / 2
        
        let saturation = delta == 0 ? 0 : delta / (1 - abs(2 * lightness - 1))
        
        let hue: Double
        if delta == 0 {
            hue = 0
        } else if max == rgb.red {
            hue = 60 * fmod(((rgb.green - rgb.blue) / delta), 6)
        } else if max == rgb.green {
            hue = 60 * ((rgb.blue - rgb.red) / delta + 2)
        } else {
            hue = 60 * ((rgb.red - rgb.green) / delta + 4)
        }
        
        return (hue: hue < 0 ? hue + 360 : hue, saturation: saturation, lightness: lightness)
    }
    
    private static func rgbToCMYK(_ rgb: (red: Double, green: Double, blue: Double)) -> (cyan: Double, magenta: Double, yellow: Double, black: Double) {
        let black = 1 - max(rgb.red, max(rgb.green, rgb.blue))
        let cyan = (1 - rgb.red - black) / (1 - black)
        let magenta = (1 - rgb.green - black) / (1 - black)
        let yellow = (1 - rgb.blue - black) / (1 - black)
        
        return (cyan: cyan, magenta: magenta, yellow: yellow, black: black)
    }
    
    private static func calculateLuminance(_ rgb: (red: Double, green: Double, blue: Double)) -> Double {
        return 0.299 * rgb.red + 0.587 * rgb.green + 0.114 * rgb.blue
    }
    
    private static func determineTemperature(_ hsv: (hue: Double, saturation: Double, value: Double)) -> ColorTemperature {
        let hue = hsv.hue * 360
        
        if hue >= 0 && hue < 60 || hue >= 300 && hue <= 360 {
            return .warm
        } else if hue >= 180 && hue < 300 {
            return .cool
        } else {
            return .neutral
        }
    }
    
    private static func getPsychologicalMeaning(_ hsv: (hue: Double, saturation: Double, value: Double)) -> String {
        let hue = hsv.hue * 360
        let saturation = hsv.saturation
        let value = hsv.value
        
        if saturation < 0.3 {
            return "Neutral, calm, sophisticated"
        } else if value < 0.3 {
            return "Dark, mysterious, elegant"
        } else if value > 0.7 {
            return "Bright, energetic, cheerful"
        } else if hue < 30 || hue > 330 {
            return "Warm, passionate, energetic"
        } else if hue >= 30 && hue < 90 {
            return "Optimistic, creative, friendly"
        } else if hue >= 90 && hue < 150 {
            return "Natural, balanced, harmonious"
        } else if hue >= 150 && hue < 210 {
            return "Calm, trustworthy, professional"
        } else if hue >= 210 && hue < 270 {
            return "Spiritual, mysterious, creative"
        } else {
            return "Romantic, feminine, gentle"
        }
    }
    
    private static func calculateAccessibilityScore(_ rgb: (red: Double, green: Double, blue: Double)) -> Int {
        let luminance = calculateLuminance(rgb)
        
        // Simple accessibility score based on luminance
        if luminance > 0.7 {
            return 90
        } else if luminance > 0.5 {
            return 70
        } else if luminance > 0.3 {
            return 50
        } else {
            return 30
        }
    }
}

// MARK: - Color Extensions

extension Color {
    func toHex() -> String {
        let rgb = self.toRGB()
        let r = Int(rgb.red * 255)
        let g = Int(rgb.green * 255)
        let b = Int(rgb.blue * 255)
        return String(format: "#%02X%02X%02X", r, g, b)
    }
}

// MARK: - Color Palette Row View

struct ColorPaletteRowView: View {
    let palette: ColorPalette
    @ObservedObject var colorManager: AdvancedColorManager
    
    var body: some View {
        HStack(spacing: 10) {
            Text(palette.name)
                .font(.subheadline)
                .foregroundColor(.primary)
            
            Spacer()
            
            HStack(spacing: 5) {
                ForEach(palette.colors.prefix(5), id: \.self) { color in
                    Circle()
                        .fill(color)
                        .frame(width: 20, height: 20)
                        .overlay(
                            Circle()
                                .stroke(Color.black, lineWidth: 1)
                        )
                }
            }
        }
        .padding(.horizontal, 10)
        .padding(.vertical, 8)
        .background(Color.white)
        .cornerRadius(8)
    }
}

// MARK: - Create Palette Dialog View

struct CreatePaletteDialogView: View {
    @ObservedObject var colorManager: AdvancedColorManager
    @Environment(\.dismiss) private var dismiss
    @State private var paletteName = ""
    @State private var selectedColors: [Color] = []
    
    var body: some View {
        NavigationView {
            VStack(spacing: 20) {
                Text("Create Color Palette")
                    .font(.title2)
                    .fontWeight(.bold)
                    .foregroundColor(.primary)
                
                VStack(spacing: 15) {
                    TextField("Palette name", text: $paletteName)
                        .textFieldStyle(RoundedBorderTextFieldStyle())
                    
                    Text("Select colors from history:")
                        .font(.headline)
                        .foregroundColor(.primary)
                    
                    ScrollView {
                        LazyVGrid(columns: Array(repeating: GridItem(.flexible()), count: 8), spacing: 8) {
                            ForEach(colorManager.colorHistory) { sample in
                                Button(action: {
                                    if selectedColors.contains(sample.color) {
                                        selectedColors.removeAll { $0 == sample.color }
                                    } else {
                                        selectedColors.append(sample.color)
                                    }
                                }) {
                                    Circle()
                                        .fill(sample.color)
                                        .frame(width: 30, height: 30)
                                        .overlay(
                                            Circle()
                                                .stroke(selectedColors.contains(sample.color) ? Color.blue : Color.black, lineWidth: 2)
                                        )
                                }
                                .buttonStyle(PlainButtonStyle())
                            }
                        }
                    }
                    .frame(maxHeight: 200)
                }
                
                Spacer()
                
                HStack(spacing: 15) {
                    Button("Cancel") {
                        dismiss()
                    }
                    .buttonStyle(.bordered)
                    
                    Button("Create") {
                        colorManager.createColorPalette(name: paletteName, colors: selectedColors)
                        dismiss()
                    }
                    .buttonStyle(.borderedProminent)
                    .disabled(paletteName.isEmpty || selectedColors.isEmpty)
                }
            }
            .padding()
            .navigationBarHidden(true)
        }
    }
}
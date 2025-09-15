import SwiftUI

// MARK: - Color Mixing System

class ColorMixingSystem: ObservableObject {
    @Published var primaryColor: Color = .red
    @Published var secondaryColor: Color = .blue
    @Published var mixedColor: Color = .purple
    @Published var customColors: [Color] = []
    @Published var showMixingPalette = false
    
    private let maxCustomColors = 20
    
    init() {
        updateMixedColor()
    }
    
    func setPrimaryColor(_ color: Color) {
        primaryColor = color
        updateMixedColor()
    }
    
    func setSecondaryColor(_ color: Color) {
        secondaryColor = color
        updateMixedColor()
    }
    
    func updateMixedColor() {
        mixedColor = mixColors(primaryColor, secondaryColor)
    }
    
    func addCustomColor(_ color: Color) {
        if customColors.count < maxCustomColors {
            customColors.append(color)
        } else {
            customColors.removeFirst()
            customColors.append(color)
        }
    }
    
    func removeCustomColor(at index: Int) {
        guard index < customColors.count else { return }
        customColors.remove(at: index)
    }
    
    private func mixColors(_ color1: Color, _ color2: Color) -> Color {
        // Convert colors to RGB components
        let rgb1 = color1.toRGB()
        let rgb2 = color2.toRGB()
        
        // Mix the colors (simple average)
        let mixedR = (rgb1.red + rgb2.red) / 2
        let mixedG = (rgb1.green + rgb2.green) / 2
        let mixedB = (rgb1.blue + rgb2.blue) / 2
        
        return Color(red: mixedR, green: mixedG, blue: mixedB)
    }
}

// MARK: - Color Extensions

extension Color {
    func toRGB() -> (red: Double, green: Double, blue: Double) {
        let uiColor = UIColor(self)
        var red: CGFloat = 0
        var green: CGFloat = 0
        var blue: CGFloat = 0
        var alpha: CGFloat = 0
        
        uiColor.getRed(&red, green: &green, blue: &blue, alpha: &alpha)
        
        return (red: Double(red), green: Double(green), blue: Double(blue))
    }
    
    func toHSV() -> (hue: Double, saturation: Double, value: Double) {
        let uiColor = UIColor(self)
        var hue: CGFloat = 0
        var saturation: CGFloat = 0
        var brightness: CGFloat = 0
        var alpha: CGFloat = 0
        
        uiColor.getHue(&hue, saturation: &saturation, brightness: &brightness, alpha: &alpha)
        
        return (hue: Double(hue), saturation: Double(saturation), value: Double(brightness))
    }
    
    static func fromRGB(red: Double, green: Double, blue: Double) -> Color {
        return Color(red: red, green: green, blue: blue)
    }
    
    static func fromHSV(hue: Double, saturation: Double, value: Double) -> Color {
        let uiColor = UIColor(hue: CGFloat(hue), saturation: CGFloat(saturation), brightness: CGFloat(value), alpha: 1.0)
        return Color(uiColor)
    }
}

// MARK: - Color Mixing Palette View

struct ColorMixingPaletteView: View {
    @ObservedObject var colorMixingSystem: ColorMixingSystem
    @ObservedObject var errorHandler: ErrorHandler
    @Binding var selectedColor: Color
    
    let baseColors: [Color] = [
        .red, .orange, .yellow, .green, .mint, .teal,
        .cyan, .blue, .indigo, .purple, .pink, .brown,
        .gray, .black, .white
    ]
    
    var body: some View {
        VStack(spacing: 15) {
            // Header
            HStack {
                Text("🎨 Color Mixing")
                    .font(.title2)
                    .fontWeight(.bold)
                    .foregroundColor(.primary)
                
                Spacer()
                
                Button(action: {
                    withAnimation(.spring()) {
                        colorMixingSystem.showMixingPalette.toggle()
                    }
                }) {
                    Image(systemName: colorMixingSystem.showMixingPalette ? "chevron.up" : "chevron.down")
                        .foregroundColor(.blue)
                }
            }
            .padding(.horizontal)
            
            if colorMixingSystem.showMixingPalette {
                // Color mixing area
                VStack(spacing: 20) {
                    // Primary and Secondary color selection
                    HStack(spacing: 30) {
                        // Primary Color
                        VStack {
                            Text("Primary")
                                .font(.caption)
                                .foregroundColor(.secondary)
                            
                            Button(action: {
                                selectedColor = colorMixingSystem.primaryColor
                                SafeHapticFeedback.shared.lightImpact()
                            }) {
                                Circle()
                                    .fill(colorMixingSystem.primaryColor)
                                    .frame(width: 60, height: 60)
                                    .overlay(
                                        Circle()
                                            .stroke(Color.black, lineWidth: 3)
                                    )
                                    .shadow(radius: 5)
                            }
                            .buttonStyle(PlainButtonStyle())
                        }
                        
                        // Mixing indicator
                        VStack {
                            Image(systemName: "plus")
                                .font(.title2)
                                .foregroundColor(.secondary)
                            
                            Text("Mix")
                                .font(.caption)
                                .foregroundColor(.secondary)
                        }
                        
                        // Secondary Color
                        VStack {
                            Text("Secondary")
                                .font(.caption)
                                .foregroundColor(.secondary)
                            
                            Button(action: {
                                selectedColor = colorMixingSystem.secondaryColor
                                SafeHapticFeedback.shared.lightImpact()
                            }) {
                                Circle()
                                    .fill(colorMixingSystem.secondaryColor)
                                    .frame(width: 60, height: 60)
                                    .overlay(
                                        Circle()
                                            .stroke(Color.black, lineWidth: 3)
                                    )
                                    .shadow(radius: 5)
                            }
                            .buttonStyle(PlainButtonStyle())
                        }
                    }
                    
                    // Mixed Color Result
                    VStack {
                        Text("Mixed Color")
                            .font(.headline)
                            .foregroundColor(.primary)
                        
                        Button(action: {
                            selectedColor = colorMixingSystem.mixedColor
                            colorMixingSystem.addCustomColor(colorMixingSystem.mixedColor)
                            SafeHapticFeedback.shared.success()
                        }) {
                            Circle()
                                .fill(colorMixingSystem.mixedColor)
                                .frame(width: 80, height: 80)
                                .overlay(
                                    Circle()
                                        .stroke(Color.black, lineWidth: 4)
                                )
                                .shadow(color: colorMixingSystem.mixedColor.opacity(0.5), radius: 10)
                                .scaleEffect(1.1)
                        }
                        .buttonStyle(PlainButtonStyle())
                        
                        Text("Tap to use")
                            .font(.caption)
                            .foregroundColor(.secondary)
                    }
                    
                    // Base colors for selection
                    VStack {
                        Text("Choose Colors to Mix")
                            .font(.subheadline)
                            .foregroundColor(.primary)
                        
                        LazyVGrid(columns: Array(repeating: GridItem(.flexible()), count: 8), spacing: 8) {
                            ForEach(baseColors, id: \.self) { color in
                                Button(action: {
                                    // Alternate between primary and secondary
                                    if colorMixingSystem.primaryColor == colorMixingSystem.secondaryColor {
                                        colorMixingSystem.setPrimaryColor(color)
                                    } else {
                                        colorMixingSystem.setSecondaryColor(color)
                                    }
                                    SafeHapticFeedback.shared.lightImpact()
                                }) {
                                    Circle()
                                        .fill(color)
                                        .frame(width: 30, height: 30)
                                        .overlay(
                                            Circle()
                                                .stroke(Color.black, lineWidth: 2)
                                        )
                                }
                                .buttonStyle(PlainButtonStyle())
                            }
                        }
                    }
                    
                    // Custom colors
                    if !colorMixingSystem.customColors.isEmpty {
                        VStack {
                            Text("Your Custom Colors")
                                .font(.subheadline)
                                .foregroundColor(.primary)
                            
                            LazyVGrid(columns: Array(repeating: GridItem(.flexible()), count: 8), spacing: 8) {
                                ForEach(Array(colorMixingSystem.customColors.enumerated()), id: \.offset) { index, color in
                                    Button(action: {
                                        selectedColor = color
                                        SafeHapticFeedback.shared.lightImpact()
                                    }) {
                                        Circle()
                                            .fill(color)
                                            .frame(width: 35, height: 35)
                                            .overlay(
                                                Circle()
                                                    .stroke(Color.black, lineWidth: 2)
                                            )
                                            .shadow(radius: 3)
                                    }
                                    .buttonStyle(PlainButtonStyle())
                                    .contextMenu {
                                        Button("Use Color") {
                                            selectedColor = color
                                        }
                                        Button("Remove", role: .destructive) {
                                            colorMixingSystem.removeCustomColor(at: index)
                                        }
                                    }
                                }
                            }
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
                colors: [Color.purple.opacity(0.05), Color.pink.opacity(0.05)],
                startPoint: .topLeading,
                endPoint: .bottomTrailing
            )
        )
    }
}

// MARK: - Advanced Color Picker

struct AdvancedColorPickerView: View {
    @Binding var selectedColor: Color
    @State private var hue: Double = 0
    @State private var saturation: Double = 1
    @State private var brightness: Double = 1
    @State private var showAdvancedPicker = false
    
    var body: some View {
        VStack(spacing: 15) {
            // Toggle button
            Button(action: {
                withAnimation(.spring()) {
                    showAdvancedPicker.toggle()
                }
            }) {
                HStack {
                    Text("🎨 Advanced Color Picker")
                        .font(.headline)
                        .foregroundColor(.primary)
                    
                    Spacer()
                    
                    Image(systemName: showAdvancedPicker ? "chevron.up" : "chevron.down")
                        .foregroundColor(.blue)
                }
                .padding(.horizontal)
            }
            
            if showAdvancedPicker {
                VStack(spacing: 20) {
                    // Current color preview
                    Circle()
                        .fill(selectedColor)
                        .frame(width: 100, height: 100)
                        .overlay(
                            Circle()
                                .stroke(Color.black, lineWidth: 4)
                        )
                        .shadow(radius: 10)
                    
                    // Hue slider
                    VStack {
                        Text("Hue")
                            .font(.subheadline)
                            .foregroundColor(.primary)
                        
                        Slider(value: $hue, in: 0...1)
                            .accentColor(.blue)
                            .onChange(of: hue) { _ in
                                updateColor()
                            }
                    }
                    
                    // Saturation slider
                    VStack {
                        Text("Saturation")
                            .font(.subheadline)
                            .foregroundColor(.primary)
                        
                        Slider(value: $saturation, in: 0...1)
                            .accentColor(.green)
                            .onChange(of: saturation) { _ in
                                updateColor()
                            }
                    }
                    
                    // Brightness slider
                    VStack {
                        Text("Brightness")
                            .font(.subheadline)
                            .foregroundColor(.primary)
                        
                        Slider(value: $brightness, in: 0...1)
                            .accentColor(.orange)
                            .onChange(of: brightness) { _ in
                                updateColor()
                            }
                    }
                    
                    // Quick color buttons
                    VStack {
                        Text("Quick Colors")
                            .font(.subheadline)
                            .foregroundColor(.primary)
                        
                        LazyVGrid(columns: Array(repeating: GridItem(.flexible()), count: 6), spacing: 10) {
                            ForEach([
                                Color.red, Color.orange, Color.yellow,
                                Color.green, Color.blue, Color.purple,
                                Color.pink, Color.brown, Color.gray,
                                Color.black, Color.white, Color.cyan
                            ], id: \.self) { color in
                                Button(action: {
                                    selectedColor = color
                                    let hsv = color.toHSV()
                                    hue = hsv.hue
                                    saturation = hsv.saturation
                                    brightness = hsv.value
                                    SafeHapticFeedback.shared.lightImpact()
                                }) {
                                    Circle()
                                        .fill(color)
                                        .frame(width: 40, height: 40)
                                        .overlay(
                                            Circle()
                                                .stroke(Color.black, lineWidth: 2)
                                        )
                                }
                                .buttonStyle(PlainButtonStyle())
                            }
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
        .onAppear {
            let hsv = selectedColor.toHSV()
            hue = hsv.hue
            saturation = hsv.saturation
            brightness = hsv.value
        }
    }
    
    private func updateColor() {
        selectedColor = Color.fromHSV(hue: hue, saturation: saturation, value: brightness)
    }
}

// MARK: - Color Palette Manager

class ColorPaletteManager: ObservableObject {
    @Published var palettes: [ColorPalette] = []
    @Published var currentPalette: ColorPalette?
    
    init() {
        loadDefaultPalettes()
    }
    
    func loadDefaultPalettes() {
        palettes = [
            ColorPalette(name: "Rainbow", colors: [
                .red, .orange, .yellow, .green, .blue, .purple
            ]),
            ColorPalette(name: "Nature", colors: [
                .green, .brown, .yellow, .orange, .red, .blue
            ]),
            ColorPalette(name: "Ocean", colors: [
                .blue, .cyan, .mint, .teal, .white, .gray
            ]),
            ColorPalette(name: "Sunset", colors: [
                .red, .orange, .yellow, .pink, .purple, .indigo
            ])
        ]
        
        currentPalette = palettes.first
    }
    
    func createCustomPalette(name: String, colors: [Color]) {
        let palette = ColorPalette(name: name, colors: colors)
        palettes.append(palette)
    }
    
    func selectPalette(_ palette: ColorPalette) {
        currentPalette = palette
    }
}

struct ColorPalette {
    let id = UUID()
    let name: String
    let colors: [Color]
}

// MARK: - Palette Selector View

struct PaletteSelectorView: View {
    @ObservedObject var paletteManager: ColorPaletteManager
    @Binding var selectedColor: Color
    
    var body: some View {
        VStack(spacing: 10) {
            Text("Color Palettes")
                .font(.headline)
                .foregroundColor(.primary)
            
            ScrollView(.horizontal, showsIndicators: false) {
                HStack(spacing: 15) {
                    ForEach(paletteManager.palettes, id: \.id) { palette in
                        Button(action: {
                            paletteManager.selectPalette(palette)
                            SafeHapticFeedback.shared.mediumImpact()
                        }) {
                            VStack {
                                HStack(spacing: 2) {
                                    ForEach(palette.colors.prefix(4), id: \.self) { color in
                                        Circle()
                                            .fill(color)
                                            .frame(width: 15, height: 15)
                                    }
                                }
                                
                                Text(palette.name)
                                    .font(.caption)
                                    .foregroundColor(.primary)
                            }
                            .padding(.horizontal, 10)
                            .padding(.vertical, 8)
                            .background(
                                RoundedRectangle(cornerRadius: 10)
                                    .fill(paletteManager.currentPalette?.id == palette.id ? Color.blue.opacity(0.2) : Color.gray.opacity(0.1))
                            )
                        }
                        .buttonStyle(PlainButtonStyle())
                    }
                }
                .padding(.horizontal)
            }
            
            // Current palette colors
            if let currentPalette = paletteManager.currentPalette {
                LazyVGrid(columns: Array(repeating: GridItem(.flexible()), count: 8), spacing: 8) {
                    ForEach(currentPalette.colors, id: \.self) { color in
                        Button(action: {
                            selectedColor = color
                            SafeHapticFeedback.shared.lightImpact()
                        }) {
                            Circle()
                                .fill(color)
                                .frame(width: 35, height: 35)
                                .overlay(
                                    Circle()
                                        .stroke(selectedColor == color ? Color.black : Color.clear, lineWidth: 3)
                                )
                                .scaleEffect(selectedColor == color ? 1.2 : 1.0)
                        }
                        .buttonStyle(PlainButtonStyle())
                    }
                }
                .padding(.horizontal)
            }
        }
        .padding(.vertical, 10)
        .background(Color.gray.opacity(0.1))
    }
}
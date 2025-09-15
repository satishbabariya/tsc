import SwiftUI
import CoreGraphics
import CoreImage

// MARK: - Filter Categories

enum FilterCategory: String, CaseIterable {
    case color = "Color"
    case blur = "Blur"
    case distortion = "Distortion"
    case stylize = "Stylize"
    case sharpen = "Sharpen"
    case noise = "Noise"
    case artistic = "Artistic"
    
    var icon: Image {
        switch self {
        case .color: return Image(systemName: "paintpalette")
        case .blur: return Image(systemName: "cloud")
        case .distortion: return Image(systemName: "waveform")
        case .stylize: return Image(systemName: "sparkles")
        case .sharpen: return Image(systemName: "target")
        case .noise: return Image(systemName: "dot.radiowaves.left.and.right")
        case .artistic: return Image(systemName: "paintbrush")
        }
    }
}

// MARK: - Filter Types

enum FilterType: String, CaseIterable {
    // Color filters
    case brightness = "brightness"
    case contrast = "contrast"
    case saturation = "saturation"
    case hue = "hue"
    case vibrance = "vibrance"
    case colorBalance = "color_balance"
    case levels = "levels"
    case curves = "curves"
    case colorize = "colorize"
    case sepia = "sepia"
    case blackAndWhite = "black_and_white"
    case invert = "invert"
    
    // Blur filters
    case gaussianBlur = "gaussian_blur"
    case motionBlur = "motion_blur"
    case radialBlur = "radial_blur"
    case zoomBlur = "zoom_blur"
    case boxBlur = "box_blur"
    
    // Distortion filters
    case bulge = "bulge"
    case pinch = "pinch"
    case twirl = "twirl"
    case wave = "wave"
    case ripple = "ripple"
    case glass = "glass"
    
    // Stylize filters
    case emboss = "emboss"
    case relief = "relief"
    case edgeWork = "edge_work"
    case crystallize = "crystallize"
    case pointillize = "pointillize"
    case posterize = "posterize"
    
    // Sharpen filters
    case sharpen = "sharpen"
    case unsharpMask = "unsharp_mask"
    case edgeEnhance = "edge_enhance"
    
    // Noise filters
    case noiseReduction = "noise_reduction"
    case addNoise = "add_noise"
    case filmGrain = "film_grain"
    
    // Artistic filters
    case oilPainting = "oil_painting"
    case watercolor = "watercolor"
    case sketch = "sketch"
    case cartoon = "cartoon"
    case vintage = "vintage"
    
    var name: String {
        switch self {
        case .brightness: return "Brightness"
        case .contrast: return "Contrast"
        case .saturation: return "Saturation"
        case .hue: return "Hue"
        case .vibrance: return "Vibrance"
        case .colorBalance: return "Color Balance"
        case .levels: return "Levels"
        case .curves: return "Curves"
        case .colorize: return "Colorize"
        case .sepia: return "Sepia"
        case .blackAndWhite: return "Black & White"
        case .invert: return "Invert"
        case .gaussianBlur: return "Gaussian Blur"
        case .motionBlur: return "Motion Blur"
        case .radialBlur: return "Radial Blur"
        case .zoomBlur: return "Zoom Blur"
        case .boxBlur: return "Box Blur"
        case .bulge: return "Bulge"
        case .pinch: return "Pinch"
        case .twirl: return "Twirl"
        case .wave: return "Wave"
        case .ripple: return "Ripple"
        case .glass: return "Glass"
        case .emboss: return "Emboss"
        case .relief: return "Relief"
        case .edgeWork: return "Edge Work"
        case .crystallize: return "Crystallize"
        case .pointillize: return "Pointillize"
        case .posterize: return "Posterize"
        case .sharpen: return "Sharpen"
        case .unsharpMask: return "Unsharp Mask"
        case .edgeEnhance: return "Edge Enhance"
        case .noiseReduction: return "Noise Reduction"
        case .addNoise: return "Add Noise"
        case .filmGrain: return "Film Grain"
        case .oilPainting: return "Oil Painting"
        case .watercolor: return "Watercolor"
        case .sketch: return "Sketch"
        case .cartoon: return "Cartoon"
        case .vintage: return "Vintage"
        }
    }
    
    var category: FilterCategory {
        switch self {
        case .brightness, .contrast, .saturation, .hue, .vibrance, .colorBalance, .levels, .curves, .colorize, .sepia, .blackAndWhite, .invert:
            return .color
        case .gaussianBlur, .motionBlur, .radialBlur, .zoomBlur, .boxBlur:
            return .blur
        case .bulge, .pinch, .twirl, .wave, .ripple, .glass:
            return .distortion
        case .emboss, .relief, .edgeWork, .crystallize, .pointillize, .posterize:
            return .stylize
        case .sharpen, .unsharpMask, .edgeEnhance:
            return .sharpen
        case .noiseReduction, .addNoise, .filmGrain:
            return .noise
        case .oilPainting, .watercolor, .sketch, .cartoon, .vintage:
            return .artistic
        }
    }
    
    var icon: Image {
        switch self {
        case .brightness: return Image(systemName: "sun.max")
        case .contrast: return Image(systemName: "circle.lefthalf.filled")
        case .saturation: return Image(systemName: "drop")
        case .hue: return Image(systemName: "rainbow")
        case .vibrance: return Image(systemName: "sparkles")
        case .colorBalance: return Image(systemName: "scale.3d")
        case .levels: return Image(systemName: "chart.bar")
        case .curves: return Image(systemName: "chart.line.uptrend.xyaxis")
        case .colorize: return Image(systemName: "paintbrush.fill")
        case .sepia: return Image(systemName: "photo")
        case .blackAndWhite: return Image(systemName: "circle.righthalf.filled")
        case .invert: return Image(systemName: "arrow.2.squarepath")
        case .gaussianBlur: return Image(systemName: "cloud")
        case .motionBlur: return Image(systemName: "arrow.right")
        case .radialBlur: return Image(systemName: "circle.dotted")
        case .zoomBlur: return Image(systemName: "magnifyingglass")
        case .boxBlur: return Image(systemName: "square")
        case .bulge: return Image(systemName: "plus.circle")
        case .pinch: return Image(systemName: "minus.circle")
        case .twirl: return Image(systemName: "arrow.clockwise")
        case .wave: return Image(systemName: "waveform")
        case .ripple: return Image(systemName: "water.waves")
        case .glass: return Image(systemName: "rectangle.portrait")
        case .emboss: return Image(systemName: "square.stack.3d.up")
        case .relief: return Image(systemName: "mountain.2")
        case .edgeWork: return Image(systemName: "square.and.line.vertical.and.square")
        case .crystallize: return Image(systemName: "diamond")
        case .pointillize: return Image(systemName: "circle.grid.3x3")
        case .posterize: return Image(systemName: "square.grid.3x3")
        case .sharpen: return Image(systemName: "target")
        case .unsharpMask: return Image(systemName: "target")
        case .edgeEnhance: return Image(systemName: "square.and.line.vertical.and.square")
        case .noiseReduction: return Image(systemName: "minus.circle")
        case .addNoise: return Image(systemName: "plus.circle")
        case .filmGrain: return Image(systemName: "photo.on.rectangle")
        case .oilPainting: return Image(systemName: "paintbrush.fill")
        case .watercolor: return Image(systemName: "drop.fill")
        case .sketch: return Image(systemName: "pencil")
        case .cartoon: return Image(systemName: "face.smiling")
        case .vintage: return Image(systemName: "camera")
        }
    }
}

// MARK: - Filter Model

struct Filter: Identifiable {
    let id = UUID()
    let type: FilterType
    let intensity: Double
    let parameters: [String: Double]
    let timestamp: Date
    
    init(type: FilterType, intensity: Double = 1.0, parameters: [String: Double] = [:]) {
        self.type = type
        self.intensity = intensity
        self.parameters = parameters
        self.timestamp = Date()
    }
}

// MARK: - Filter Manager

class FilterManager: ObservableObject {
    @Published var selectedCategory: FilterCategory = .color
    @Published var selectedFilter: FilterType = .brightness
    @Published var appliedFilters: [Filter] = []
    @Published var showFilterPanel = false
    @Published var filterIntensity: Double = 1.0
    @Published var filterParameters: [String: Double] = [:]
    @Published var previewMode = false
    
    func applyFilter() {
        let filter = Filter(
            type: selectedFilter,
            intensity: filterIntensity,
            parameters: filterParameters
        )
        appliedFilters.append(filter)
    }
    
    func removeFilter(_ filter: Filter) {
        appliedFilters.removeAll { $0.id == filter.id }
    }
    
    func clearAllFilters() {
        appliedFilters.removeAll()
    }
    
    func resetFilterSettings() {
        filterIntensity = 1.0
        filterParameters = [:]
    }
    
    func getDefaultParameters(for filterType: FilterType) -> [String: Double] {
        switch filterType {
        case .brightness:
            return ["brightness": 0.0]
        case .contrast:
            return ["contrast": 1.0]
        case .saturation:
            return ["saturation": 1.0]
        case .hue:
            return ["hue": 0.0]
        case .vibrance:
            return ["vibrance": 0.0]
        case .colorBalance:
            return ["red": 0.0, "green": 0.0, "blue": 0.0]
        case .levels:
            return ["inputMin": 0.0, "inputMax": 1.0, "outputMin": 0.0, "outputMax": 1.0]
        case .curves:
            return ["point1": 0.0, "point2": 0.5, "point3": 1.0]
        case .colorize:
            return ["hue": 0.0, "saturation": 1.0]
        case .sepia:
            return ["intensity": 1.0]
        case .gaussianBlur:
            return ["radius": 5.0]
        case .motionBlur:
            return ["radius": 10.0, "angle": 0.0]
        case .radialBlur:
            return ["radius": 10.0, "angle": 0.0]
        case .bulge:
            return ["radius": 100.0, "scale": 0.5]
        case .pinch:
            return ["radius": 100.0, "scale": 0.5]
        case .twirl:
            return ["radius": 100.0, "angle": 1.0]
        case .wave:
            return ["wavelength": 20.0, "amplitude": 10.0]
        case .emboss:
            return ["intensity": 1.0]
        case .crystallize:
            return ["radius": 20.0]
        case .sharpen:
            return ["intensity": 1.0]
        case .unsharpMask:
            return ["radius": 2.0, "intensity": 1.0]
        case .noiseReduction:
            return ["noiseLevel": 0.02, "sharpness": 0.4]
        case .addNoise:
            return ["noiseLevel": 0.1]
        case .oilPainting:
            return ["radius": 4.0, "intensity": 1.0]
        case .watercolor:
            return ["radius": 8.0, "intensity": 1.0]
        case .sketch:
            return ["intensity": 1.0]
        case .cartoon:
            return ["intensity": 1.0]
        case .vintage:
            return ["intensity": 1.0]
        default:
            return [:]
        }
    }
}

// MARK: - Filter Panel View

struct FilterPanelView: View {
    @ObservedObject var filterManager: FilterManager
    @ObservedObject var errorHandler: ErrorHandler
    
    var body: some View {
        VStack(spacing: 15) {
            // Header
            HStack {
                Text("🎨 Filters & Effects")
                    .font(.title2)
                    .fontWeight(.bold)
                    .foregroundColor(.primary)
                
                Spacer()
                
                Button(action: {
                    withAnimation(.spring()) {
                        filterManager.showFilterPanel.toggle()
                    }
                }) {
                    Image(systemName: filterManager.showFilterPanel ? "chevron.up" : "chevron.down")
                        .foregroundColor(.blue)
                }
            }
            .padding(.horizontal)
            
            if filterManager.showFilterPanel {
                VStack(spacing: 15) {
                    // Category selector
                    ScrollView(.horizontal, showsIndicators: false) {
                        HStack(spacing: 15) {
                            ForEach(FilterCategory.allCases, id: \.self) { category in
                                Button(action: {
                                    filterManager.selectedCategory = category
                                    SafeHapticFeedback.shared.lightImpact()
                                }) {
                                    VStack {
                                        category.icon
                                            .font(.system(size: 20))
                                            .foregroundColor(filterManager.selectedCategory == category ? .white : .primary)
                                        
                                        Text(category.rawValue)
                                            .font(.caption)
                                            .foregroundColor(filterManager.selectedCategory == category ? .white : .primary)
                                    }
                                    .padding(.horizontal, 12)
                                    .padding(.vertical, 8)
                                    .background(
                                        RoundedRectangle(cornerRadius: 10)
                                            .fill(filterManager.selectedCategory == category ? Color.blue : Color.gray.opacity(0.2))
                                    )
                                }
                                .buttonStyle(PlainButtonStyle())
                            }
                        }
                        .padding(.horizontal)
                    }
                    
                    // Filter selector
                    ScrollView(.horizontal, showsIndicators: false) {
                        HStack(spacing: 15) {
                            ForEach(FilterType.allCases.filter { $0.category == filterManager.selectedCategory }, id: \.self) { filter in
                                Button(action: {
                                    filterManager.selectedFilter = filter
                                    filterManager.filterParameters = filterManager.getDefaultParameters(for: filter)
                                    SafeHapticFeedback.shared.lightImpact()
                                }) {
                                    VStack {
                                        filter.icon
                                            .font(.system(size: 25))
                                            .foregroundColor(filterManager.selectedFilter == filter ? .white : .primary)
                                            .frame(width: 50, height: 50)
                                            .background(
                                                Circle()
                                                    .fill(filterManager.selectedFilter == filter ? Color.blue : Color.gray.opacity(0.2))
                                            )
                                        
                                        Text(filter.name)
                                            .font(.caption)
                                            .foregroundColor(.primary)
                                    }
                                }
                                .buttonStyle(PlainButtonStyle())
                            }
                        }
                        .padding(.horizontal)
                    }
                    
                    // Filter controls
                    FilterControlsView(filterManager: filterManager)
                    
                    // Action buttons
                    HStack(spacing: 15) {
                        Button("Preview") {
                            filterManager.previewMode.toggle()
                            SafeHapticFeedback.shared.lightImpact()
                        }
                        .buttonStyle(.bordered)
                        
                        Button("Apply") {
                            filterManager.applyFilter()
                            SafeHapticFeedback.shared.success()
                        }
                        .buttonStyle(.borderedProminent)
                        
                        Button("Reset") {
                            filterManager.resetFilterSettings()
                            SafeHapticFeedback.shared.lightImpact()
                        }
                        .buttonStyle(.bordered)
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

// MARK: - Filter Controls View

struct FilterControlsView: View {
    @ObservedObject var filterManager: FilterManager
    
    var body: some View {
        VStack(spacing: 15) {
            Text(filterManager.selectedFilter.name)
                .font(.headline)
                .foregroundColor(.primary)
            
            // Intensity control
            VStack {
                HStack {
                    Text("Intensity: \(Int(filterManager.filterIntensity * 100))%")
                        .font(.subheadline)
                        .foregroundColor(.primary)
                    
                    Spacer()
                    
                    Text("\(Int(filterManager.filterIntensity * 100))%")
                        .font(.caption)
                        .foregroundColor(.secondary)
                }
                
                Slider(value: $filterManager.filterIntensity, in: 0.0...2.0)
                    .accentColor(.blue)
            }
            
            // Filter-specific parameters
            FilterParameterControlsView(filterManager: filterManager)
        }
    }
}

// MARK: - Filter Parameter Controls View

struct FilterParameterControlsView: View {
    @ObservedObject var filterManager: FilterManager
    
    var body: some View {
        VStack(spacing: 10) {
            ForEach(Array(filterManager.filterParameters.keys.sorted()), id: \.self) { key in
                VStack {
                    HStack {
                        Text("\(key.capitalized): \(Int(filterManager.filterParameters[key] ?? 0))")
                            .font(.subheadline)
                            .foregroundColor(.primary)
                        
                        Spacer()
                        
                        Text("\(Int(filterManager.filterParameters[key] ?? 0))")
                            .font(.caption)
                            .foregroundColor(.secondary)
                    }
                    
                    Slider(
                        value: Binding(
                            get: { filterManager.filterParameters[key] ?? 0 },
                            set: { filterManager.filterParameters[key] = $0 }
                        ),
                        in: getRange(for: key)
                    )
                    .accentColor(.green)
                }
            }
        }
    }
    
    private func getRange(for key: String) -> ClosedRange<Double> {
        switch key {
        case "brightness": return -1.0...1.0
        case "contrast": return 0.0...3.0
        case "saturation": return 0.0...3.0
        case "hue": return -180.0...180.0
        case "vibrance": return -1.0...1.0
        case "red", "green", "blue": return -1.0...1.0
        case "radius": return 0.0...100.0
        case "angle": return -180.0...180.0
        case "scale": return -2.0...2.0
        case "wavelength": return 1.0...50.0
        case "amplitude": return 0.0...50.0
        case "intensity": return 0.0...2.0
        case "noiseLevel": return 0.0...1.0
        case "sharpness": return 0.0...1.0
        default: return 0.0...1.0
        }
    }
}

// MARK: - Applied Filters View

struct AppliedFiltersView: View {
    @ObservedObject var filterManager: FilterManager
    
    var body: some View {
        VStack(spacing: 10) {
            Text("Applied Filters")
                .font(.headline)
                .foregroundColor(.primary)
            
            if filterManager.appliedFilters.isEmpty {
                Text("No filters applied")
                    .font(.subheadline)
                    .foregroundColor(.secondary)
            } else {
                ScrollView {
                    LazyVStack(spacing: 8) {
                        ForEach(filterManager.appliedFilters) { filter in
                            AppliedFilterRowView(filter: filter, filterManager: filterManager)
                        }
                    }
                }
                .frame(maxHeight: 200)
            }
            
            if !filterManager.appliedFilters.isEmpty {
                Button("Clear All Filters") {
                    filterManager.clearAllFilters()
                    SafeHapticFeedback.shared.clear()
                }
                .buttonStyle(.bordered)
            }
        }
        .padding()
        .background(Color.gray.opacity(0.1))
        .cornerRadius(10)
    }
}

// MARK: - Applied Filter Row View

struct AppliedFilterRowView: View {
    let filter: Filter
    @ObservedObject var filterManager: FilterManager
    
    var body: some View {
        HStack(spacing: 10) {
            filter.type.icon
                .font(.system(size: 20))
                .foregroundColor(.blue)
            
            VStack(alignment: .leading, spacing: 2) {
                Text(filter.type.name)
                    .font(.subheadline)
                    .foregroundColor(.primary)
                
                Text("Intensity: \(Int(filter.intensity * 100))%")
                    .font(.caption)
                    .foregroundColor(.secondary)
            }
            
            Spacer()
            
            Button(action: {
                filterManager.removeFilter(filter)
            }) {
                Image(systemName: "trash")
                    .foregroundColor(.red)
                    .font(.system(size: 16))
            }
        }
        .padding(.horizontal, 10)
        .padding(.vertical, 8)
        .background(Color.white)
        .cornerRadius(8)
    }
}

// MARK: - Filter Engine

class FilterEngine {
    static func applyFilter(_ filter: Filter, to image: UIImage) -> UIImage? {
        guard let ciImage = CIImage(image: image) else { return nil }
        
        let context = CIContext()
        var outputImage: CIImage = ciImage
        
        switch filter.type {
        case .brightness:
            outputImage = applyBrightnessFilter(ciImage, intensity: filter.intensity, brightness: filter.parameters["brightness"] ?? 0)
        case .contrast:
            outputImage = applyContrastFilter(ciImage, intensity: filter.intensity, contrast: filter.parameters["contrast"] ?? 1)
        case .saturation:
            outputImage = applySaturationFilter(ciImage, intensity: filter.intensity, saturation: filter.parameters["saturation"] ?? 1)
        case .hue:
            outputImage = applyHueFilter(ciImage, intensity: filter.intensity, hue: filter.parameters["hue"] ?? 0)
        case .gaussianBlur:
            outputImage = applyGaussianBlurFilter(ciImage, intensity: filter.intensity, radius: filter.parameters["radius"] ?? 5)
        case .sepia:
            outputImage = applySepiaFilter(ciImage, intensity: filter.intensity)
        case .invert:
            outputImage = applyInvertFilter(ciImage, intensity: filter.intensity)
        case .emboss:
            outputImage = applyEmbossFilter(ciImage, intensity: filter.intensity)
        case .sharpen:
            outputImage = applySharpenFilter(ciImage, intensity: filter.intensity)
        default:
            return image // Return original if filter not implemented
        }
        
        guard let cgImage = context.createCGImage(outputImage, from: outputImage.extent) else { return nil }
        return UIImage(cgImage: cgImage)
    }
    
    // Individual filter implementations
    private static func applyBrightnessFilter(_ image: CIImage, intensity: Double, brightness: Double) -> CIImage {
        let filter = CIFilter.colorControls()
        filter.inputImage = image
        filter.brightness = Float(brightness * intensity)
        return filter.outputImage ?? image
    }
    
    private static func applyContrastFilter(_ image: CIImage, intensity: Double, contrast: Double) -> CIImage {
        let filter = CIFilter.colorControls()
        filter.inputImage = image
        filter.contrast = Float(contrast * intensity)
        return filter.outputImage ?? image
    }
    
    private static func applySaturationFilter(_ image: CIImage, intensity: Double, saturation: Double) -> CIImage {
        let filter = CIFilter.colorControls()
        filter.inputImage = image
        filter.saturation = Float(saturation * intensity)
        return filter.outputImage ?? image
    }
    
    private static func applyHueFilter(_ image: CIImage, intensity: Double, hue: Double) -> CIImage {
        let filter = CIFilter.hueAdjust()
        filter.inputImage = image
        filter.angle = Float(hue * intensity * .pi / 180)
        return filter.outputImage ?? image
    }
    
    private static func applyGaussianBlurFilter(_ image: CIImage, intensity: Double, radius: Double) -> CIImage {
        let filter = CIFilter.gaussianBlur()
        filter.inputImage = image
        filter.radius = Float(radius * intensity)
        return filter.outputImage ?? image
    }
    
    private static func applySepiaFilter(_ image: CIImage, intensity: Double) -> CIImage {
        let filter = CIFilter.sepiaTone()
        filter.inputImage = image
        filter.intensity = Float(intensity)
        return filter.outputImage ?? image
    }
    
    private static func applyInvertFilter(_ image: CIImage, intensity: Double) -> CIImage {
        let filter = CIFilter.colorInvert()
        filter.inputImage = image
        // Note: colorInvert doesn't have intensity, so we'll blend with original
        if intensity < 1.0 {
            let blendFilter = CIFilter.sourceOverCompositing()
            blendFilter.inputImage = filter.outputImage
            blendFilter.backgroundImage = image
            return blendFilter.outputImage ?? image
        }
        return filter.outputImage ?? image
    }
    
    private static func applyEmbossFilter(_ image: CIImage, intensity: Double) -> CIImage {
        let filter = CIFilter.convolution3X3()
        filter.inputImage = image
        filter.weights = CIVector(values: [-2, -1, 0, -1, 1, 1, 0, 1, 2], count: 9)
        filter.bias = Float(intensity)
        return filter.outputImage ?? image
    }
    
    private static func applySharpenFilter(_ image: CIImage, intensity: Double) -> CIImage {
        let filter = CIFilter.sharpenLuminance()
        filter.inputImage = image
        filter.sharpness = Float(intensity)
        return filter.outputImage ?? image
    }
}

// MARK: - Filter Presets

struct FilterPresets {
    static let vintage = Filter(type: .vintage, intensity: 0.8)
    static let blackAndWhite = Filter(type: .blackAndWhite, intensity: 1.0)
    static let sepia = Filter(type: .sepia, intensity: 0.7)
    static let oilPainting = Filter(type: .oilPainting, intensity: 0.6)
    static let watercolor = Filter(type: .watercolor, intensity: 0.8)
    static let sketch = Filter(type: .sketch, intensity: 0.7)
    static let cartoon = Filter(type: .cartoon, intensity: 0.8)
    
    static let allPresets: [Filter] = [
        vintage, blackAndWhite, sepia, oilPainting, watercolor, sketch, cartoon
    ]
}
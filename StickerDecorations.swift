import SwiftUI

// MARK: - Sticker Types

enum StickerType: String, CaseIterable {
    case star = "star"
    case heart = "heart"
    case flower = "flower"
    case butterfly = "butterfly"
    case sun = "sun"
    case moon = "moon"
    case cloud = "cloud"
    case rainbow = "rainbow"
    case sparkle = "sparkle"
    case diamond = "diamond"
    case circle = "circle"
    case square = "square"
    case triangle = "triangle"
    case arrow = "arrow"
    case checkmark = "checkmark"
    case exclamation = "exclamation"
    case question = "question"
    
    var icon: Image {
        switch self {
        case .star: return Image(systemName: "star.fill")
        case .heart: return Image(systemName: "heart.fill")
        case .flower: return Image(systemName: "leaf.fill")
        case .butterfly: return Image(systemName: "butterfly")
        case .sun: return Image(systemName: "sun.max.fill")
        case .moon: return Image(systemName: "moon.fill")
        case .cloud: return Image(systemName: "cloud.fill")
        case .rainbow: return Image(systemName: "cloud.rainbow.fill")
        case .sparkle: return Image(systemName: "sparkles")
        case .diamond: return Image(systemName: "diamond.fill")
        case .circle: return Image(systemName: "circle.fill")
        case .square: return Image(systemName: "square.fill")
        case .triangle: return Image(systemName: "triangle.fill")
        case .arrow: return Image(systemName: "arrow.right")
        case .checkmark: return Image(systemName: "checkmark")
        case .exclamation: return Image(systemName: "exclamationmark")
        case .question: return Image(systemName: "questionmark")
        }
    }
    
    var category: StickerCategory {
        switch self {
        case .star, .heart, .diamond, .sparkle:
            return .shapes
        case .flower, .butterfly:
            return .nature
        case .sun, .moon, .cloud, .rainbow:
            return .weather
        case .circle, .square, .triangle:
            return .basic
        case .arrow, .checkmark, .exclamation, .question:
            return .symbols
        }
    }
}

enum StickerCategory: String, CaseIterable {
    case shapes = "Shapes"
    case nature = "Nature"
    case weather = "Weather"
    case basic = "Basic"
    case symbols = "Symbols"
    
    var icon: Image {
        switch self {
        case .shapes: return Image(systemName: "star.fill")
        case .nature: return Image(systemName: "leaf.fill")
        case .weather: return Image(systemName: "cloud.sun.fill")
        case .basic: return Image(systemName: "circle.fill")
        case .symbols: return Image(systemName: "exclamationmark")
        }
    }
}

// MARK: - Sticker Model

struct Sticker: Identifiable, Equatable {
    let id = UUID()
    let type: StickerType
    var position: CGPoint
    var size: CGFloat
    var rotation: Double
    var color: Color
    var isSelected: Bool = false
    
    static func == (lhs: Sticker, rhs: Sticker) -> Bool {
        return lhs.id == rhs.id
    }
}

// MARK: - Sticker Manager

class StickerManager: ObservableObject {
    @Published var stickers: [Sticker] = []
    @Published var selectedSticker: Sticker?
    @Published var showStickerPalette = false
    @Published var currentStickerType: StickerType = .star
    @Published var currentStickerSize: CGFloat = 30
    @Published var currentStickerColor: Color = .red
    
    func addSticker(at position: CGPoint) {
        let newSticker = Sticker(
            type: currentStickerType,
            position: position,
            size: currentStickerSize,
            rotation: 0,
            color: currentStickerColor
        )
        
        stickers.append(newSticker)
        SafeHapticFeedback.shared.lightImpact()
    }
    
    func removeSticker(_ sticker: Sticker) {
        stickers.removeAll { $0.id == sticker.id }
        SafeHapticFeedback.shared.mediumImpact()
    }
    
    func updateSticker(_ sticker: Sticker) {
        if let index = stickers.firstIndex(where: { $0.id == sticker.id }) {
            stickers[index] = sticker
        }
    }
    
    func clearAllStickers() {
        stickers.removeAll()
        SafeHapticFeedback.shared.heavyImpact()
    }
    
    func selectSticker(_ sticker: Sticker) {
        // Deselect all stickers
        for i in 0..<stickers.count {
            stickers[i].isSelected = false
        }
        
        // Select the chosen sticker
        if let index = stickers.firstIndex(where: { $0.id == sticker.id }) {
            stickers[index].isSelected = true
            selectedSticker = stickers[index]
        }
    }
    
    func deselectAllStickers() {
        for i in 0..<stickers.count {
            stickers[i].isSelected = false
        }
        selectedSticker = nil
    }
}

// MARK: - Sticker Palette View

struct StickerPaletteView: View {
    @ObservedObject var stickerManager: StickerManager
    @ObservedObject var errorHandler: ErrorHandler
    @State private var selectedCategory: StickerCategory = .shapes
    
    var body: some View {
        VStack(spacing: 15) {
            // Header
            HStack {
                Text("✨ Stickers")
                    .font(.title2)
                    .fontWeight(.bold)
                    .foregroundColor(.primary)
                
                Spacer()
                
                Button(action: {
                    withAnimation(.spring()) {
                        stickerManager.showStickerPalette.toggle()
                    }
                }) {
                    Image(systemName: stickerManager.showStickerPalette ? "chevron.up" : "chevron.down")
                        .foregroundColor(.purple)
                }
            }
            .padding(.horizontal)
            
            if stickerManager.showStickerPalette {
                VStack(spacing: 20) {
                    // Category selector
                    ScrollView(.horizontal, showsIndicators: false) {
                        HStack(spacing: 15) {
                            ForEach(StickerCategory.allCases, id: \.self) { category in
                                Button(action: {
                                    selectedCategory = category
                                    SafeHapticFeedback.shared.lightImpact()
                                }) {
                                    VStack {
                                        category.icon
                                            .font(.system(size: 20))
                                            .foregroundColor(selectedCategory == category ? .white : .primary)
                                        
                                        Text(category.rawValue)
                                            .font(.caption)
                                            .foregroundColor(selectedCategory == category ? .white : .primary)
                                    }
                                    .padding(.horizontal, 12)
                                    .padding(.vertical, 8)
                                    .background(
                                        RoundedRectangle(cornerRadius: 10)
                                            .fill(selectedCategory == category ? Color.purple : Color.gray.opacity(0.2))
                                    )
                                }
                                .buttonStyle(PlainButtonStyle())
                            }
                        }
                        .padding(.horizontal)
                    }
                    
                    // Sticker type selector
                    LazyVGrid(columns: Array(repeating: GridItem(.flexible()), count: 6), spacing: 10) {
                        ForEach(StickerType.allCases.filter { $0.category == selectedCategory }, id: \.self) { stickerType in
                            Button(action: {
                                stickerManager.currentStickerType = stickerType
                                SafeHapticFeedback.shared.lightImpact()
                            }) {
                                VStack {
                                    stickerType.icon
                                        .font(.system(size: 25))
                                        .foregroundColor(stickerManager.currentStickerType == stickerType ? .white : .primary)
                                        .frame(width: 40, height: 40)
                                        .background(
                                            Circle()
                                                .fill(stickerManager.currentStickerType == stickerType ? Color.purple : Color.gray.opacity(0.2))
                                        )
                                }
                            }
                            .buttonStyle(PlainButtonStyle())
                        }
                    }
                    
                    // Size and color controls
                    VStack(spacing: 15) {
                        // Size control
                        VStack {
                            Text("Size: \(Int(stickerManager.currentStickerSize))")
                                .font(.subheadline)
                                .foregroundColor(.primary)
                            
                            Slider(value: $stickerManager.currentStickerSize, in: 20...80)
                                .accentColor(.purple)
                        }
                        
                        // Color control
                        VStack {
                            Text("Color")
                                .font(.subheadline)
                                .foregroundColor(.primary)
                            
                            LazyVGrid(columns: Array(repeating: GridItem(.flexible()), count: 8), spacing: 8) {
                                ForEach([
                                    Color.red, Color.orange, Color.yellow, Color.green,
                                    Color.blue, Color.purple, Color.pink, Color.brown,
                                    Color.gray, Color.black, Color.white, Color.cyan
                                ], id: \.self) { color in
                                    Button(action: {
                                        stickerManager.currentStickerColor = color
                                        SafeHapticFeedback.shared.lightImpact()
                                    }) {
                                        Circle()
                                            .fill(color)
                                            .frame(width: 25, height: 25)
                                            .overlay(
                                                Circle()
                                                    .stroke(stickerManager.currentStickerColor == color ? Color.black : Color.clear, lineWidth: 2)
                                            )
                                    }
                                    .buttonStyle(PlainButtonStyle())
                                }
                            }
                        }
                    }
                    
                    // Action buttons
                    HStack(spacing: 15) {
                        Button(action: {
                            stickerManager.clearAllStickers()
                        }) {
                            HStack {
                                Image(systemName: "trash")
                                Text("Clear All")
                            }
                            .foregroundColor(.white)
                            .padding(.horizontal, 15)
                            .padding(.vertical, 8)
                            .background(Color.red)
                            .cornerRadius(15)
                        }
                        
                        Spacer()
                        
                        Text("Tap on canvas to add stickers")
                            .font(.caption)
                            .foregroundColor(.secondary)
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

// MARK: - Sticker View

struct StickerView: View {
    let sticker: Sticker
    @ObservedObject var stickerManager: StickerManager
    @State private var dragOffset: CGSize = .zero
    @State private var rotationAngle: Double = 0
    
    var body: some View {
        sticker.type.icon
            .font(.system(size: sticker.size))
            .foregroundColor(sticker.color)
            .rotationEffect(.degrees(sticker.rotation + rotationAngle))
            .scaleEffect(sticker.isSelected ? 1.2 : 1.0)
            .overlay(
                Circle()
                    .stroke(sticker.isSelected ? Color.blue : Color.clear, lineWidth: 3)
                    .scaleEffect(sticker.isSelected ? 1.2 : 1.0)
            )
            .position(
                x: sticker.position.x + dragOffset.width,
                y: sticker.position.y + dragOffset.height
            )
            .onTapGesture {
                stickerManager.selectSticker(sticker)
            }
            .onLongPressGesture {
                // Delete sticker on long press
                stickerManager.removeSticker(sticker)
            }
            .gesture(
                DragGesture()
                    .onChanged { value in
                        dragOffset = value.translation
                    }
                    .onEnded { value in
                        let newPosition = CGPoint(
                            x: sticker.position.x + value.translation.width,
                            y: sticker.position.y + value.translation.height
                        )
                        
                        var updatedSticker = sticker
                        updatedSticker.position = newPosition
                        stickerManager.updateSticker(updatedSticker)
                        
                        dragOffset = .zero
                    }
            )
            .gesture(
                RotationGesture()
                    .onChanged { value in
                        rotationAngle = value.degrees
                    }
                    .onEnded { value in
                        var updatedSticker = sticker
                        updatedSticker.rotation += value.degrees
                        stickerManager.updateSticker(updatedSticker)
                        
                        rotationAngle = 0
                    }
            )
            .animation(.spring(response: 0.3, dampingFraction: 0.6), value: sticker.isSelected)
    }
}

// MARK: - Enhanced Canvas with Stickers

struct StickerCanvasView: View {
    let template: ColoringTemplate
    @Binding var selectedColor: Color
    @ObservedObject var errorHandler: ErrorHandler
    @StateObject private var stickerManager = StickerManager()
    @StateObject private var undoRedoManager = UndoRedoManager()
    @State private var showShareSheet = false
    @State private var capturedImage: UIImage?
    
    var body: some View {
        GeometryReader { geometry in
            ZStack {
                // Background
                RoundedRectangle(cornerRadius: 20)
                    .fill(Color.white)
                    .shadow(color: .black.opacity(0.1), radius: 5, x: 0, y: 2)
                
                VStack {
                    // Template drawing with colored regions and stickers
                    ZStack {
                        // Colored regions (behind the outline)
                        ForEach(template.regions, id: \.id) { region in
                            region.path
                                .fill(undoRedoManager.coloredRegions[region.id] ?? Color.clear)
                                .scaleEffect(min(geometry.size.width / 400, geometry.size.height / 400))
                                .onTapGesture {
                                    // Validate region before coloring
                                    guard InputValidator.validateRegionId(region.id) else {
                                        errorHandler.handleError(.invalidRegion)
                                        return
                                    }
                                    
                                    withAnimation(.spring(response: 0.3, dampingFraction: 0.6)) {
                                        undoRedoManager.colorRegion(region.id, with: selectedColor)
                                    }
                                    
                                    SafeHapticFeedback.shared.lightImpact()
                                }
                        }
                        
                        // Template outline (on top)
                        template.drawing
                            .stroke(Color.black, lineWidth: 3)
                            .scaleEffect(min(geometry.size.width / 400, geometry.size.height / 400))
                        
                        // Stickers
                        ForEach(stickerManager.stickers) { sticker in
                            StickerView(sticker: sticker, stickerManager: stickerManager)
                        }
                    }
                    .frame(maxWidth: .infinity, maxHeight: .infinity)
                    .background(Color.white)
                    .cornerRadius(15)
                    .padding()
                    .onTapGesture { location in
                        // Add sticker at tap location
                        let adjustedLocation = CGPoint(
                            x: location.x,
                            y: location.y
                        )
                        stickerManager.addSticker(at: adjustedLocation)
                    }
                    
                    // Undo/Redo buttons
                    UndoRedoButtonsView(undoRedoManager: undoRedoManager, errorHandler: errorHandler)
                    
                    // Action buttons
                    HStack(spacing: 20) {
                        // Clear button
                        Button(action: {
                            withAnimation(.spring()) {
                                undoRedoManager.clearAll()
                                stickerManager.clearAllStickers()
                            }
                            SafeHapticFeedback.shared.mediumImpact()
                        }) {
                            HStack {
                                Image(systemName: "trash")
                                Text("Clear")
                            }
                            .foregroundColor(.white)
                            .padding(.horizontal, 20)
                            .padding(.vertical, 10)
                            .background(Color.red)
                            .cornerRadius(20)
                        }
                        
                        // Save/Share button
                        Button(action: {
                            guard let image = captureImage() else {
                                errorHandler.handleError(.imageCaptureFailed)
                                return
                            }
                            capturedImage = image
                            showShareSheet = true
                        }) {
                            HStack {
                                Image(systemName: "square.and.arrow.up")
                                Text("Share")
                            }
                            .foregroundColor(.white)
                            .padding(.horizontal, 20)
                            .padding(.vertical, 10)
                            .background(Color.blue)
                            .cornerRadius(20)
                        }
                    }
                    .padding(.bottom, 10)
                }
            }
        }
        .onChange(of: template) { _ in
            withAnimation(.easeInOut) {
                undoRedoManager.reset()
                stickerManager.clearAllStickers()
            }
        }
        .sheet(isPresented: $showShareSheet) {
            if let image = capturedImage {
                SafeShareSheet(activityItems: [image], errorHandler: errorHandler)
            }
        }
    }
    
    private func captureImage() -> UIImage? {
        let content = ZStack {
            Color.white
            VStack {
                ZStack {
                    // Colored regions
                    ForEach(template.regions, id: \.id) { region in
                        region.path
                            .fill(undoRedoManager.coloredRegions[region.id] ?? Color.clear)
                    }
                    
                    // Template outline
                    template.drawing
                        .stroke(Color.black, lineWidth: 3)
                    
                    // Stickers
                    ForEach(stickerManager.stickers) { sticker in
                        sticker.type.icon
                            .font(.system(size: sticker.size))
                            .foregroundColor(sticker.color)
                            .rotationEffect(.degrees(sticker.rotation))
                            .position(sticker.position)
                    }
                }
                .frame(width: 400, height: 400)
            }
        }
        
        return SafeImageCapture.captureImage(from: content, errorHandler: errorHandler)
    }
}

// MARK: - Sticker Animation Effects

struct AnimatedStickerView: View {
    let sticker: Sticker
    @State private var isAnimating = false
    
    var body: some View {
        sticker.type.icon
            .font(.system(size: sticker.size))
            .foregroundColor(sticker.color)
            .rotationEffect(.degrees(sticker.rotation))
            .scaleEffect(isAnimating ? 1.1 : 1.0)
            .animation(
                .easeInOut(duration: 1.0).repeatForever(autoreverses: true),
                value: isAnimating
            )
            .onAppear {
                isAnimating = true
            }
    }
}
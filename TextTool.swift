import SwiftUI
import CoreGraphics

// MARK: - Text Element Model

class TextElement: ObservableObject, Identifiable {
    let id = UUID()
    @Published var text: String
    @Published var position: CGPoint
    @Published var fontSize: CGFloat
    @Published var fontName: String
    @Published var color: Color
    @Published var isBold: Bool
    @Published var isItalic: Bool
    @Published var isUnderlined: Bool
    @Published var alignment: TextAlignment
    @Published var rotation: Double
    @Published var opacity: Double
    @Published var isSelected: Bool = false
    @Published var bounds: CGRect
    
    init(text: String = "Text", position: CGPoint = CGPoint(x: 200, y: 200)) {
        self.text = text
        self.position = position
        self.fontSize = 24
        self.fontName = "Helvetica"
        self.color = .black
        self.isBold = false
        self.isItalic = false
        self.isUnderlined = false
        self.alignment = .center
        self.rotation = 0
        self.opacity = 1.0
        self.bounds = CGRect(x: position.x - 50, y: position.y - 12, width: 100, height: 24)
    }
    
    func updateBounds() {
        let textSize = calculateTextSize()
        bounds = CGRect(
            x: position.x - textSize.width / 2,
            y: position.y - textSize.height / 2,
            width: textSize.width,
            height: textSize.height
        )
    }
    
    private func calculateTextSize() -> CGSize {
        let font = UIFont(name: fontName, size: fontSize) ?? UIFont.systemFont(ofSize: fontSize)
        let attributes: [NSAttributedString.Key: Any] = [
            .font: font,
            .underlineStyle: isUnderlined ? NSUnderlineStyle.single.rawValue : 0
        ]
        
        let attributedString = NSAttributedString(string: text, attributes: attributes)
        let size = attributedString.boundingRect(
            with: CGSize(width: CGFloat.greatestFiniteMagnitude, height: CGFloat.greatestFiniteMagnitude),
            options: [.usesLineFragmentOrigin, .usesFontLeading],
            context: nil
        ).size
        
        return size
    }
}

// MARK: - Text Tool Manager

class TextToolManager: ObservableObject {
    @Published var textElements: [TextElement] = []
    @Published var selectedTextElement: TextElement?
    @Published var isTextMode = false
    @Published var showTextPanel = false
    @Published var currentFont: String = "Helvetica"
    @Published var currentFontSize: CGFloat = 24
    @Published var currentColor: Color = .black
    @Published var currentAlignment: TextAlignment = .center
    
    // Font settings
    @Published var isBold = false
    @Published var isItalic = false
    @Published var isUnderlined = false
    @Published var textOpacity: Double = 1.0
    @Published var textRotation: Double = 0
    
    func addTextElement(at position: CGPoint) {
        let textElement = TextElement(position: position)
        textElement.fontName = currentFont
        textElement.fontSize = currentFontSize
        textElement.color = currentColor
        textElement.alignment = currentAlignment
        textElement.isBold = isBold
        textElement.isItalic = isItalic
        textElement.isUnderlined = isUnderlined
        textElement.opacity = textOpacity
        textElement.rotation = textRotation
        
        textElements.append(textElement)
        selectedTextElement = textElement
    }
    
    func selectTextElement(_ element: TextElement) {
        // Deselect all elements
        for textElement in textElements {
            textElement.isSelected = false
        }
        
        // Select the chosen element
        element.isSelected = true
        selectedTextElement = element
        
        // Update current settings
        currentFont = element.fontName
        currentFontSize = element.fontSize
        currentColor = element.color
        currentAlignment = element.alignment
        isBold = element.isBold
        isItalic = element.isItalic
        isUnderlined = element.isUnderlined
        textOpacity = element.opacity
        textRotation = element.rotation
    }
    
    func deleteTextElement(_ element: TextElement) {
        textElements.removeAll { $0.id == element.id }
        if selectedTextElement?.id == element.id {
            selectedTextElement = nil
        }
    }
    
    func updateSelectedTextElement() {
        guard let element = selectedTextElement else { return }
        
        element.fontName = currentFont
        element.fontSize = currentFontSize
        element.color = currentColor
        element.alignment = currentAlignment
        element.isBold = isBold
        element.isItalic = isItalic
        element.isUnderlined = isUnderlined
        element.opacity = textOpacity
        element.rotation = textRotation
        
        element.updateBounds()
    }
    
    func clearAllText() {
        textElements.removeAll()
        selectedTextElement = nil
    }
}

// MARK: - Text Tool Panel

struct TextToolPanel: View {
    @ObservedObject var textToolManager: TextToolManager
    @ObservedObject var errorHandler: ErrorHandler
    
    var body: some View {
        VStack(spacing: 15) {
            // Header
            HStack {
                Text("📝 Text Tool")
                    .font(.title2)
                    .fontWeight(.bold)
                    .foregroundColor(.primary)
                
                Spacer()
                
                Button(action: {
                    withAnimation(.spring()) {
                        textToolManager.showTextPanel.toggle()
                    }
                }) {
                    Image(systemName: textToolManager.showTextPanel ? "chevron.up" : "chevron.down")
                        .foregroundColor(.blue)
                }
            }
            .padding(.horizontal)
            
            if textToolManager.showTextPanel {
                VStack(spacing: 15) {
                    // Text input
                    VStack {
                        Text("Text Content")
                            .font(.headline)
                            .foregroundColor(.primary)
                        
                        TextField("Enter text", text: Binding(
                            get: { textToolManager.selectedTextElement?.text ?? "" },
                            set: { textToolManager.selectedTextElement?.text = $0 }
                        ))
                        .textFieldStyle(RoundedBorderTextFieldStyle())
                        .onChange(of: textToolManager.selectedTextElement?.text) { _ in
                            textToolManager.updateSelectedTextElement()
                        }
                    }
                    
                    // Font selection
                    VStack {
                        Text("Font")
                            .font(.headline)
                            .foregroundColor(.primary)
                        
                        ScrollView(.horizontal, showsIndicators: false) {
                            HStack(spacing: 10) {
                                ForEach(AvailableFonts.allFonts, id: \.self) { font in
                                    Button(action: {
                                        textToolManager.currentFont = font
                                        textToolManager.updateSelectedTextElement()
                                    }) {
                                        Text("Aa")
                                            .font(.custom(font, size: 20))
                                            .foregroundColor(textToolManager.currentFont == font ? .white : .primary)
                                            .padding(.horizontal, 12)
                                            .padding(.vertical, 8)
                                            .background(
                                                RoundedRectangle(cornerRadius: 8)
                                                    .fill(textToolManager.currentFont == font ? Color.blue : Color.gray.opacity(0.2))
                                            )
                                    }
                                    .buttonStyle(PlainButtonStyle())
                                }
                            }
                            .padding(.horizontal)
                        }
                    }
                    
                    // Font size
                    VStack {
                        HStack {
                            Text("Size: \(Int(textToolManager.currentFontSize))")
                                .font(.subheadline)
                                .foregroundColor(.primary)
                            
                            Spacer()
                            
                            Text("\(Int(textToolManager.currentFontSize))pt")
                                .font(.caption)
                                .foregroundColor(.secondary)
                        }
                        
                        Slider(value: $textToolManager.currentFontSize, in: 8...72)
                            .accentColor(.blue)
                            .onChange(of: textToolManager.currentFontSize) { _ in
                                textToolManager.updateSelectedTextElement()
                            }
                    }
                    
                    // Text formatting
                    VStack {
                        Text("Formatting")
                            .font(.headline)
                            .foregroundColor(.primary)
                        
                        HStack(spacing: 15) {
                            // Bold toggle
                            Button(action: {
                                textToolManager.isBold.toggle()
                                textToolManager.updateSelectedTextElement()
                            }) {
                                Image(systemName: "bold")
                                    .font(.system(size: 20))
                                    .foregroundColor(textToolManager.isBold ? .white : .primary)
                                    .frame(width: 40, height: 40)
                                    .background(
                                        Circle()
                                            .fill(textToolManager.isBold ? Color.blue : Color.gray.opacity(0.2))
                                    )
                            }
                            .buttonStyle(PlainButtonStyle())
                            
                            // Italic toggle
                            Button(action: {
                                textToolManager.isItalic.toggle()
                                textToolManager.updateSelectedTextElement()
                            }) {
                                Image(systemName: "italic")
                                    .font(.system(size: 20))
                                    .foregroundColor(textToolManager.isItalic ? .white : .primary)
                                    .frame(width: 40, height: 40)
                                    .background(
                                        Circle()
                                            .fill(textToolManager.isItalic ? Color.blue : Color.gray.opacity(0.2))
                                    )
                            }
                            .buttonStyle(PlainButtonStyle())
                            
                            // Underline toggle
                            Button(action: {
                                textToolManager.isUnderlined.toggle()
                                textToolManager.updateSelectedTextElement()
                            }) {
                                Image(systemName: "underline")
                                    .font(.system(size: 20))
                                    .foregroundColor(textToolManager.isUnderlined ? .white : .primary)
                                    .frame(width: 40, height: 40)
                                    .background(
                                        Circle()
                                            .fill(textToolManager.isUnderlined ? Color.blue : Color.gray.opacity(0.2))
                                    )
                            }
                            .buttonStyle(PlainButtonStyle())
                        }
                    }
                    
                    // Text alignment
                    VStack {
                        Text("Alignment")
                            .font(.headline)
                            .foregroundColor(.primary)
                        
                        HStack(spacing: 10) {
                            ForEach([TextAlignment.left, TextAlignment.center, TextAlignment.right], id: \.self) { alignment in
                                Button(action: {
                                    textToolManager.currentAlignment = alignment
                                    textToolManager.updateSelectedTextElement()
                                }) {
                                    Image(systemName: alignmentIcon(alignment))
                                        .font(.system(size: 20))
                                        .foregroundColor(textToolManager.currentAlignment == alignment ? .white : .primary)
                                        .frame(width: 40, height: 40)
                                        .background(
                                            Circle()
                                                .fill(textToolManager.currentAlignment == alignment ? Color.blue : Color.gray.opacity(0.2))
                                        )
                                }
                                .buttonStyle(PlainButtonStyle())
                            }
                        }
                    }
                    
                    // Color selection
                    VStack {
                        Text("Color")
                            .font(.headline)
                            .foregroundColor(.primary)
                        
                        LazyVGrid(columns: Array(repeating: GridItem(.flexible()), count: 8), spacing: 8) {
                            ForEach([
                                Color.black, Color.white, Color.red, Color.blue,
                                Color.green, Color.yellow, Color.orange, Color.purple,
                                Color.pink, Color.brown, Color.gray, Color.cyan
                            ], id: \.self) { color in
                                Button(action: {
                                    textToolManager.currentColor = color
                                    textToolManager.updateSelectedTextElement()
                                }) {
                                    Circle()
                                        .fill(color)
                                        .frame(width: 30, height: 30)
                                        .overlay(
                                            Circle()
                                                .stroke(textToolManager.currentColor == color ? Color.black : Color.clear, lineWidth: 2)
                                        )
                                }
                                .buttonStyle(PlainButtonStyle())
                            }
                        }
                    }
                    
                    // Opacity and rotation
                    VStack {
                        HStack {
                            VStack {
                                Text("Opacity")
                                    .font(.subheadline)
                                    .foregroundColor(.primary)
                                
                                Slider(value: $textToolManager.textOpacity, in: 0.1...1.0)
                                    .accentColor(.green)
                                    .onChange(of: textToolManager.textOpacity) { _ in
                                        textToolManager.updateSelectedTextElement()
                                    }
                            }
                            
                            VStack {
                                Text("Rotation")
                                    .font(.subheadline)
                                    .foregroundColor(.primary)
                                
                                Slider(value: $textToolManager.textRotation, in: -180...180)
                                    .accentColor(.orange)
                                    .onChange(of: textToolManager.textRotation) { _ in
                                        textToolManager.updateSelectedTextElement()
                                    }
                            }
                        }
                    }
                    
                    // Action buttons
                    HStack(spacing: 15) {
                        Button("Add Text") {
                            textToolManager.addTextElement(at: CGPoint(x: 200, y: 200))
                            SafeHapticFeedback.shared.lightImpact()
                        }
                        .buttonStyle(.borderedProminent)
                        
                        Button("Delete") {
                            if let selected = textToolManager.selectedTextElement {
                                textToolManager.deleteTextElement(selected)
                                SafeHapticFeedback.shared.clear()
                            }
                        }
                        .buttonStyle(.bordered)
                        .disabled(textToolManager.selectedTextElement == nil)
                        
                        Button("Clear All") {
                            textToolManager.clearAllText()
                            SafeHapticFeedback.shared.clear()
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
                colors: [Color.cyan.opacity(0.05), Color.blue.opacity(0.05)],
                startPoint: .topLeading,
                endPoint: .bottomTrailing
            )
        )
    }
    
    private func alignmentIcon(_ alignment: TextAlignment) -> String {
        switch alignment {
        case .left: return "text.alignleft"
        case .center: return "text.aligncenter"
        case .right: return "text.alignright"
        }
    }
}

// MARK: - Text Element View

struct TextElementView: View {
    @ObservedObject var textElement: TextElement
    @ObservedObject var textToolManager: TextToolManager
    @State private var dragOffset: CGSize = .zero
    @State private var isDragging = false
    
    var body: some View {
        Text(textElement.text)
            .font(.custom(textElement.fontName, size: textElement.fontSize))
            .fontWeight(textElement.isBold ? .bold : .regular)
            .italic(textElement.isItalic)
            .underline(textElement.isUnderlined)
            .foregroundColor(textElement.color)
            .opacity(textElement.opacity)
            .rotationEffect(.degrees(textElement.rotation))
            .multilineTextAlignment(textElement.alignment)
            .position(
                x: textElement.position.x + dragOffset.width,
                y: textElement.position.y + dragOffset.height
            )
            .overlay(
                // Selection indicator
                Rectangle()
                    .stroke(textElement.isSelected ? Color.blue : Color.clear, lineWidth: 2)
                    .frame(width: textElement.bounds.width + 10, height: textElement.bounds.height + 10)
                    .position(
                        x: textElement.position.x + dragOffset.width,
                        y: textElement.position.y + dragOffset.height
                    )
            )
            .onTapGesture {
                textToolManager.selectTextElement(textElement)
            }
            .gesture(
                DragGesture()
                    .onChanged { value in
                        if !isDragging {
                            isDragging = true
                        }
                        dragOffset = value.translation
                    }
                    .onEnded { value in
                        textElement.position = CGPoint(
                            x: textElement.position.x + value.translation.width,
                            y: textElement.position.y + value.translation.height
                        )
                        textElement.updateBounds()
                        dragOffset = .zero
                        isDragging = false
                    }
            )
    }
}

// MARK: - Text Canvas View

struct TextCanvasView: View {
    @ObservedObject var textToolManager: TextToolManager
    @ObservedObject var errorHandler: ErrorHandler
    @State private var canvasSize: CGSize = CGSize(width: 400, height: 400)
    
    var body: some View {
        GeometryReader { geometry in
            ZStack {
                // Canvas background
                RoundedRectangle(cornerRadius: 20)
                    .fill(Color.white)
                    .shadow(color: .black.opacity(0.1), radius: 5, x: 0, y: 2)
                
                VStack {
                    // Canvas area
                    ZStack {
                        // Canvas
                        Rectangle()
                            .fill(Color.white)
                            .frame(width: canvasSize.width, height: canvasSize.height)
                            .border(Color.gray.opacity(0.3), width: 1)
                        
                        // Text elements
                        ForEach(textToolManager.textElements) { textElement in
                            TextElementView(textElement: textElement, textToolManager: textToolManager)
                        }
                    }
                    .frame(maxWidth: .infinity, maxHeight: .infinity)
                    .background(Color.white)
                    .cornerRadius(15)
                    .padding()
                    .onTapGesture { location in
                        if textToolManager.isTextMode {
                            textToolManager.addTextElement(at: location)
                        }
                    }
                    
                    // Text info
                    HStack {
                        Text("Text Elements: \(textToolManager.textElements.count)")
                            .font(.caption)
                            .foregroundColor(.secondary)
                        
                        Spacer()
                        
                        if let selected = textToolManager.selectedTextElement {
                            Text("Selected: \(selected.text)")
                                .font(.caption)
                                .foregroundColor(.blue)
                        }
                    }
                    .padding(.horizontal)
                    .padding(.bottom, 10)
                }
            }
        }
    }
}

// MARK: - Available Fonts

struct AvailableFonts {
    static let allFonts = [
        "Helvetica",
        "Helvetica-Bold",
        "Helvetica-Oblique",
        "Times New Roman",
        "Times New Roman Bold",
        "Times New Roman Italic",
        "Courier",
        "Courier-Bold",
        "Courier-Oblique",
        "Arial",
        "Arial-Bold",
        "Arial-Italic",
        "Georgia",
        "Georgia-Bold",
        "Georgia-Italic",
        "Verdana",
        "Verdana-Bold",
        "Verdana-Italic",
        "Trebuchet MS",
        "Trebuchet MS Bold",
        "Trebuchet MS Italic",
        "Comic Sans MS",
        "Comic Sans MS Bold",
        "Impact",
        "Palatino",
        "Palatino-Bold",
        "Palatino-Italic"
    ]
    
    static let kidFriendlyFonts = [
        "Comic Sans MS",
        "Comic Sans MS Bold",
        "Marker Felt",
        "Chalkduster",
        "Bradley Hand",
        "Noteworthy",
        "Papyrus",
        "Party LET",
        "Savoye LET"
    ]
    
    static let professionalFonts = [
        "Helvetica",
        "Helvetica-Bold",
        "Times New Roman",
        "Times New Roman Bold",
        "Arial",
        "Arial-Bold",
        "Georgia",
        "Georgia-Bold",
        "Verdana",
        "Verdana-Bold"
    ]
}

// MARK: - Text Effects

enum TextEffect: String, CaseIterable {
    case shadow = "shadow"
    case outline = "outline"
    case glow = "glow"
    case gradient = "gradient"
    case emboss = "emboss"
    case engrave = "engrave"
    
    var name: String {
        switch self {
        case .shadow: return "Shadow"
        case .outline: return "Outline"
        case .glow: return "Glow"
        case .gradient: return "Gradient"
        case .emboss: return "Emboss"
        case .engrave: return "Engrave"
        }
    }
    
    var icon: Image {
        switch self {
        case .shadow: return Image(systemName: "shadow")
        case .outline: return Image(systemName: "circle")
        case .glow: return Image(systemName: "sun.max")
        case .gradient: return Image(systemName: "paintbrush")
        case .emboss: return Image(systemName: "square.stack.3d.up")
        case .engrave: return Image(systemName: "square.stack.3d.down")
        }
    }
}

// MARK: - Text Templates

struct TextTemplates {
    static let birthday = "Happy Birthday!"
    static let congratulations = "Congratulations!"
    static let thankYou = "Thank You!"
    static let welcome = "Welcome!"
    static let love = "Love"
    static let friendship = "Friendship"
    static let family = "Family"
    static let adventure = "Adventure"
    static let dream = "Dream"
    static let hope = "Hope"
    
    static let allTemplates = [
        birthday, congratulations, thankYou, welcome,
        love, friendship, family, adventure, dream, hope
    ]
}

// MARK: - Text Statistics

extension TextToolManager {
    var totalTextElements: Int {
        return textElements.count
    }
    
    var totalCharacters: Int {
        return textElements.reduce(0) { total, element in
            total + element.text.count
        }
    }
    
    var mostUsedFont: String {
        let fontCounts = Dictionary(grouping: textElements, by: { $0.fontName })
            .mapValues { $0.count }
        
        return fontCounts.max(by: { $0.value < $1.value })?.key ?? "Helvetica"
    }
    
    var averageFontSize: CGFloat {
        guard !textElements.isEmpty else { return 24 }
        
        let totalSize = textElements.reduce(0) { total, element in
            total + element.fontSize
        }
        
        return totalSize / CGFloat(textElements.count)
    }
}
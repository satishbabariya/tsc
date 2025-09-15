import SwiftUI
import AVFoundation

// MARK: - Audio Manager

class AudioManager: ObservableObject {
    @Published var isSoundEnabled = true
    @Published var isMusicEnabled = true
    @Published var soundVolume: Float = 0.7
    @Published var musicVolume: Float = 0.5
    @Published var currentMusicTrack: MusicTrack = .happy
    @Published var isPlayingMusic = false
    
    private var audioPlayer: AVAudioPlayer?
    private var musicPlayer: AVAudioPlayer?
    private var soundEffects: [SoundEffect: AVAudioPlayer] = [:]
    
    init() {
        setupAudioSession()
        loadSoundEffects()
    }
    
    private func setupAudioSession() {
        do {
            try AVAudioSession.sharedInstance().setCategory(.playback, mode: .default)
            try AVAudioSession.sharedInstance().setActive(true)
        } catch {
            print("Failed to setup audio session: \(error)")
        }
    }
    
    private func loadSoundEffects() {
        for soundEffect in SoundEffect.allCases {
            if let url = Bundle.main.url(forResource: soundEffect.rawValue, withExtension: "wav") {
                do {
                    let player = try AVAudioPlayer(contentsOf: url)
                    player.prepareToPlay()
                    soundEffects[soundEffect] = player
                } catch {
                    print("Failed to load sound effect \(soundEffect.rawValue): \(error)")
                }
            }
        }
    }
    
    func playSoundEffect(_ soundEffect: SoundEffect) {
        guard isSoundEnabled else { return }
        
        if let player = soundEffects[soundEffect] {
            player.volume = soundVolume
            player.play()
        } else {
            // Fallback to system sound
            AudioServicesPlaySystemSound(soundEffect.systemSoundID)
        }
    }
    
    func playMusic(_ track: MusicTrack) {
        guard isMusicEnabled else { return }
        
        stopMusic()
        
        if let url = Bundle.main.url(forResource: track.rawValue, withExtension: "mp3") {
            do {
                musicPlayer = try AVAudioPlayer(contentsOf: url)
                musicPlayer?.volume = musicVolume
                musicPlayer?.numberOfLoops = -1 // Loop indefinitely
                musicPlayer?.play()
                currentMusicTrack = track
                isPlayingMusic = true
            } catch {
                print("Failed to play music \(track.rawValue): \(error)")
            }
        }
    }
    
    func stopMusic() {
        musicPlayer?.stop()
        musicPlayer = nil
        isPlayingMusic = false
    }
    
    func pauseMusic() {
        musicPlayer?.pause()
        isPlayingMusic = false
    }
    
    func resumeMusic() {
        musicPlayer?.play()
        isPlayingMusic = true
    }
    
    func setSoundVolume(_ volume: Float) {
        soundVolume = max(0, min(1, volume))
        for player in soundEffects.values {
            player.volume = soundVolume
        }
    }
    
    func setMusicVolume(_ volume: Float) {
        musicVolume = max(0, min(1, volume))
        musicPlayer?.volume = musicVolume
    }
}

// MARK: - Sound Effects

enum SoundEffect: String, CaseIterable {
    case colorFill = "color_fill"
    case buttonTap = "button_tap"
    case stickerAdd = "sticker_add"
    case stickerRemove = "sticker_remove"
    case undo = "undo"
    case redo = "redo"
    case clear = "clear"
    case success = "success"
    case error = "error"
    case achievement = "achievement"
    
    var systemSoundID: SystemSoundID {
        switch self {
        case .colorFill: return 1104 // Tock
        case .buttonTap: return 1103 // Tink
        case .stickerAdd: return 1105 // Tock
        case .stickerRemove: return 1106 // Tock
        case .undo: return 1107 // Tock
        case .redo: return 1108 // Tock
        case .clear: return 1109 // Tock
        case .success: return 1110 // Tock
        case .error: return 1111 // Tock
        case .achievement: return 1112 // Tock
        }
    }
}

// MARK: - Music Tracks

enum MusicTrack: String, CaseIterable {
    case happy = "happy_music"
    case peaceful = "peaceful_music"
    case playful = "playful_music"
    case nature = "nature_sounds"
    case classical = "classical_music"
    
    var name: String {
        switch self {
        case .happy: return "Happy Tunes"
        case .peaceful: return "Peaceful Melody"
        case .playful: return "Playful Beats"
        case .nature: return "Nature Sounds"
        case .classical: return "Classical Music"
        }
    }
    
    var icon: Image {
        switch self {
        case .happy: return Image(systemName: "face.smiling")
        case .peaceful: return Image(systemName: "leaf")
        case .playful: return Image(systemName: "gamecontroller")
        case .nature: return Image(systemName: "tree")
        case .classical: return Image(systemName: "music.note")
        }
    }
}

// MARK: - Audio Controls View

struct AudioControlsView: View {
    @ObservedObject var audioManager: AudioManager
    @State private var showAudioSettings = false
    
    var body: some View {
        VStack(spacing: 15) {
            // Header
            HStack {
                Text("🎵 Audio")
                    .font(.title2)
                    .fontWeight(.bold)
                    .foregroundColor(.primary)
                
                Spacer()
                
                Button(action: {
                    withAnimation(.spring()) {
                        showAudioSettings.toggle()
                    }
                }) {
                    Image(systemName: showAudioSettings ? "chevron.up" : "chevron.down")
                        .foregroundColor(.blue)
                }
            }
            .padding(.horizontal)
            
            if showAudioSettings {
                VStack(spacing: 20) {
                    // Sound Effects Toggle
                    HStack {
                        Text("Sound Effects")
                            .font(.subheadline)
                            .foregroundColor(.primary)
                        
                        Spacer()
                        
                        Toggle("", isOn: $audioManager.isSoundEnabled)
                            .toggleStyle(SwitchToggleStyle(tint: .blue))
                    }
                    
                    // Music Toggle
                    HStack {
                        Text("Background Music")
                            .font(.subheadline)
                            .foregroundColor(.primary)
                        
                        Spacer()
                        
                        Toggle("", isOn: $audioManager.isMusicEnabled)
                            .toggleStyle(SwitchToggleStyle(tint: .blue))
                    }
                    
                    // Sound Volume
                    if audioManager.isSoundEnabled {
                        VStack {
                            HStack {
                                Text("Sound Volume")
                                    .font(.subheadline)
                                    .foregroundColor(.primary)
                                
                                Spacer()
                                
                                Text("\(Int(audioManager.soundVolume * 100))%")
                                    .font(.caption)
                                    .foregroundColor(.secondary)
                            }
                            
                            Slider(value: Binding(
                                get: { Double(audioManager.soundVolume) },
                                set: { audioManager.setSoundVolume(Float($0)) }
                            ), in: 0...1)
                            .accentColor(.blue)
                        }
                    }
                    
                    // Music Volume
                    if audioManager.isMusicEnabled {
                        VStack {
                            HStack {
                                Text("Music Volume")
                                    .font(.subheadline)
                                    .foregroundColor(.primary)
                                
                                Spacer()
                                
                                Text("\(Int(audioManager.musicVolume * 100))%")
                                    .font(.caption)
                                    .foregroundColor(.secondary)
                            }
                            
                            Slider(value: Binding(
                                get: { Double(audioManager.musicVolume) },
                                set: { audioManager.setMusicVolume(Float($0)) }
                            ), in: 0...1)
                            .accentColor(.green)
                        }
                    }
                    
                    // Music Track Selection
                    if audioManager.isMusicEnabled {
                        VStack {
                            Text("Music Track")
                                .font(.subheadline)
                                .foregroundColor(.primary)
                            
                            ScrollView(.horizontal, showsIndicators: false) {
                                HStack(spacing: 15) {
                                    ForEach(MusicTrack.allCases, id: \.self) { track in
                                        Button(action: {
                                            audioManager.playMusic(track)
                                            SafeHapticFeedback.shared.lightImpact()
                                        }) {
                                            VStack {
                                                track.icon
                                                    .font(.system(size: 25))
                                                    .foregroundColor(audioManager.currentMusicTrack == track ? .white : .primary)
                                                
                                                Text(track.name)
                                                    .font(.caption)
                                                    .foregroundColor(audioManager.currentMusicTrack == track ? .white : .primary)
                                            }
                                            .padding(.horizontal, 12)
                                            .padding(.vertical, 8)
                                            .background(
                                                RoundedRectangle(cornerRadius: 10)
                                                    .fill(audioManager.currentMusicTrack == track ? Color.blue : Color.gray.opacity(0.2))
                                            )
                                        }
                                        .buttonStyle(PlainButtonStyle())
                                    }
                                }
                                .padding(.horizontal)
                            }
                            
                            // Music Control Buttons
                            HStack(spacing: 20) {
                                Button(action: {
                                    if audioManager.isPlayingMusic {
                                        audioManager.pauseMusic()
                                    } else {
                                        audioManager.resumeMusic()
                                    }
                                    SafeHapticFeedback.shared.lightImpact()
                                }) {
                                    Image(systemName: audioManager.isPlayingMusic ? "pause.circle.fill" : "play.circle.fill")
                                        .font(.system(size: 30))
                                        .foregroundColor(.blue)
                                }
                                
                                Button(action: {
                                    audioManager.stopMusic()
                                    SafeHapticFeedback.shared.lightImpact()
                                }) {
                                    Image(systemName: "stop.circle.fill")
                                        .font(.system(size: 30))
                                        .foregroundColor(.red)
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
                colors: [Color.blue.opacity(0.05), Color.green.opacity(0.05)],
                startPoint: .topLeading,
                endPoint: .bottomTrailing
            )
        )
    }
}

// MARK: - Enhanced Haptic Feedback with Audio

class EnhancedHapticFeedback {
    static let shared = EnhancedHapticFeedback()
    private var audioManager: AudioManager?
    
    func setAudioManager(_ audioManager: AudioManager) {
        self.audioManager = audioManager
    }
    
    func lightImpact() {
        // Haptic feedback
        if #available(iOS 13.0, *) {
            let impactFeedback = UIImpactFeedbackGenerator(style: .light)
            impactFeedback.impactOccurred()
        }
        
        // Sound effect
        audioManager?.playSoundEffect(.buttonTap)
    }
    
    func mediumImpact() {
        // Haptic feedback
        if #available(iOS 13.0, *) {
            let impactFeedback = UIImpactFeedbackGenerator(style: .medium)
            impactFeedback.impactOccurred()
        }
        
        // Sound effect
        audioManager?.playSoundEffect(.buttonTap)
    }
    
    func heavyImpact() {
        // Haptic feedback
        if #available(iOS 13.0, *) {
            let impactFeedback = UIImpactFeedbackGenerator(style: .heavy)
            impactFeedback.impactOccurred()
        }
        
        // Sound effect
        audioManager?.playSoundEffect(.buttonTap)
    }
    
    func success() {
        // Haptic feedback
        if #available(iOS 13.0, *) {
            let notificationFeedback = UINotificationFeedbackGenerator()
            notificationFeedback.notificationOccurred(.success)
        }
        
        // Sound effect
        audioManager?.playSoundEffect(.success)
    }
    
    func error() {
        // Haptic feedback
        if #available(iOS 13.0, *) {
            let notificationFeedback = UINotificationFeedbackGenerator()
            notificationFeedback.notificationOccurred(.error)
        }
        
        // Sound effect
        audioManager?.playSoundEffect(.error)
    }
    
    func colorFill() {
        // Haptic feedback
        if #available(iOS 13.0, *) {
            let impactFeedback = UIImpactFeedbackGenerator(style: .light)
            impactFeedback.impactOccurred()
        }
        
        // Sound effect
        audioManager?.playSoundEffect(.colorFill)
    }
    
    func stickerAdd() {
        // Haptic feedback
        if #available(iOS 13.0, *) {
            let impactFeedback = UIImpactFeedbackGenerator(style: .light)
            impactFeedback.impactOccurred()
        }
        
        // Sound effect
        audioManager?.playSoundEffect(.stickerAdd)
    }
    
    func stickerRemove() {
        // Haptic feedback
        if #available(iOS 13.0, *) {
            let impactFeedback = UIImpactFeedbackGenerator(style: .medium)
            impactFeedback.impactOccurred()
        }
        
        // Sound effect
        audioManager?.playSoundEffect(.stickerRemove)
    }
    
    func undo() {
        // Haptic feedback
        if #available(iOS 13.0, *) {
            let impactFeedback = UIImpactFeedbackGenerator(style: .light)
            impactFeedback.impactOccurred()
        }
        
        // Sound effect
        audioManager?.playSoundEffect(.undo)
    }
    
    func redo() {
        // Haptic feedback
        if #available(iOS 13.0, *) {
            let impactFeedback = UIImpactFeedbackGenerator(style: .light)
            impactFeedback.impactOccurred()
        }
        
        // Sound effect
        audioManager?.playSoundEffect(.redo)
    }
    
    func clear() {
        // Haptic feedback
        if #available(iOS 13.0, *) {
            let impactFeedback = UIImpactFeedbackGenerator(style: .heavy)
            impactFeedback.impactOccurred()
        }
        
        // Sound effect
        audioManager?.playSoundEffect(.clear)
    }
    
    func achievement() {
        // Haptic feedback
        if #available(iOS 13.0, *) {
            let notificationFeedback = UINotificationFeedbackGenerator()
            notificationFeedback.notificationOccurred(.success)
        }
        
        // Sound effect
        audioManager?.playSoundEffect(.achievement)
    }
}

// MARK: - Audio Settings Storage

class AudioSettingsStorage: ObservableObject {
    @Published var isSoundEnabled: Bool {
        didSet {
            UserDefaults.standard.set(isSoundEnabled, forKey: "isSoundEnabled")
        }
    }
    
    @Published var isMusicEnabled: Bool {
        didSet {
            UserDefaults.standard.set(isMusicEnabled, forKey: "isMusicEnabled")
        }
    }
    
    @Published var soundVolume: Float {
        didSet {
            UserDefaults.standard.set(soundVolume, forKey: "soundVolume")
        }
    }
    
    @Published var musicVolume: Float {
        didSet {
            UserDefaults.standard.set(musicVolume, forKey: "musicVolume")
        }
    }
    
    @Published var currentMusicTrack: MusicTrack {
        didSet {
            UserDefaults.standard.set(currentMusicTrack.rawValue, forKey: "currentMusicTrack")
        }
    }
    
    init() {
        self.isSoundEnabled = UserDefaults.standard.object(forKey: "isSoundEnabled") as? Bool ?? true
        self.isMusicEnabled = UserDefaults.standard.object(forKey: "isMusicEnabled") as? Bool ?? true
        self.soundVolume = UserDefaults.standard.object(forKey: "soundVolume") as? Float ?? 0.7
        self.musicVolume = UserDefaults.standard.object(forKey: "musicVolume") as? Float ?? 0.5
        
        if let trackString = UserDefaults.standard.string(forKey: "currentMusicTrack"),
           let track = MusicTrack(rawValue: trackString) {
            self.currentMusicTrack = track
        } else {
            self.currentMusicTrack = .happy
        }
    }
}

// MARK: - Audio Visualizer

struct AudioVisualizerView: View {
    @ObservedObject var audioManager: AudioManager
    @State private var animationOffset: CGFloat = 0
    
    var body: some View {
        if audioManager.isPlayingMusic {
            HStack(spacing: 2) {
                ForEach(0..<5, id: \.self) { index in
                    RoundedRectangle(cornerRadius: 2)
                        .fill(Color.blue)
                        .frame(width: 4, height: 20)
                        .scaleEffect(y: 1 + sin(animationOffset + Double(index) * 0.5) * 0.5)
                        .animation(
                            .easeInOut(duration: 0.5).repeatForever(autoreverses: true),
                            value: animationOffset
                        )
                }
            }
            .onAppear {
                animationOffset = 0
            }
        }
    }
}